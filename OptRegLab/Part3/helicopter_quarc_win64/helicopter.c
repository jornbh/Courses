/*
 * helicopter.c
 *
 * Code generation for model "helicopter".
 *
 * Model version              : 1.209
 * Simulink Coder version : 8.6 (R2014a) 27-Dec-2013
 * C source code generated on : Wed Feb 28 11:13:53 2018
 *
 * Target selection: quarc_win64.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: 32-bit Generic
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */
#include "helicopter.h"
#include "helicopter_private.h"
#include "helicopter_dt.h"

/* Block signals (auto storage) */
B_helicopter_T helicopter_B;

/* Continuous states */
X_helicopter_T helicopter_X;

/* Block states (auto storage) */
DW_helicopter_T helicopter_DW;

/* Real-time model */
RT_MODEL_helicopter_T helicopter_M_;
RT_MODEL_helicopter_T *const helicopter_M = &helicopter_M_;

/*
 * Writes out MAT-file header.  Returns success or failure.
 * Returns:
 *      0 - success
 *      1 - failure
 */
int_T rt_WriteMat4FileHeader(FILE *fp, int32_T m, int32_T n, const char *name)
{
  typedef enum { ELITTLE_ENDIAN, EBIG_ENDIAN } ByteOrder;

  int16_T one = 1;
  ByteOrder byteOrder = (*((int8_T *)&one)==1) ? ELITTLE_ENDIAN : EBIG_ENDIAN;
  int32_T type = (byteOrder == ELITTLE_ENDIAN) ? 0: 1000;
  int32_T imagf = 0;
  int32_T name_len = (int32_T)strlen(name) + 1;
  if ((fwrite(&type, sizeof(int32_T), 1, fp) == 0) ||
      (fwrite(&m, sizeof(int32_T), 1, fp) == 0) ||
      (fwrite(&n, sizeof(int32_T), 1, fp) == 0) ||
      (fwrite(&imagf, sizeof(int32_T), 1, fp) == 0) ||
      (fwrite(&name_len, sizeof(int32_T), 1, fp) == 0) ||
      (fwrite(name, sizeof(char), name_len, fp) == 0)) {
    return(1);
  } else {
    return(0);
  }
}                                      /* end rt_WriteMat4FileHeader */

/*
 * This function updates continuous states using the ODE1 fixed-step
 * solver algorithm
 */
static void rt_ertODEUpdateContinuousStates(RTWSolverInfo *si )
{
  time_T tnew = rtsiGetSolverStopTime(si);
  time_T h = rtsiGetStepSize(si);
  real_T *x = rtsiGetContStates(si);
  ODE1_IntgData *id = (ODE1_IntgData *)rtsiGetSolverData(si);
  real_T *f0 = id->f[0];
  int_T i;
  int_T nXc = 4;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);
  rtsiSetdX(si, f0);
  helicopter_derivatives();
  rtsiSetT(si, tnew);
  for (i = 0; i < nXc; i++) {
    *x += h * f0[i];
    x++;
  }

  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

/* Model output function */
void helicopter_output(void)
{
  /* local block i/o variables */
  real_T rtb_Sum4[4];
  real_T rtb_Derivative;
  real_T rtb_Backgain;
  real_T rtb_HILReadEncoderTimebase_o1;
  real_T rtb_HILReadEncoderTimebase_o2;
  real_T rtb_HILReadEncoderTimebase_o3;
  real_T *lastU;
  real_T lastTime;
  if (rtmIsMajorTimeStep(helicopter_M)) {
    /* set solver stop time */
    if (!(helicopter_M->Timing.clockTick0+1)) {
      rtsiSetSolverStopTime(&helicopter_M->solverInfo,
                            ((helicopter_M->Timing.clockTickH0 + 1) *
        helicopter_M->Timing.stepSize0 * 4294967296.0));
    } else {
      rtsiSetSolverStopTime(&helicopter_M->solverInfo,
                            ((helicopter_M->Timing.clockTick0 + 1) *
        helicopter_M->Timing.stepSize0 + helicopter_M->Timing.clockTickH0 *
        helicopter_M->Timing.stepSize0 * 4294967296.0));
    }
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(helicopter_M)) {
    helicopter_M->Timing.t[0] = rtsiGetT(&helicopter_M->solverInfo);
  }

  if (rtmIsMajorTimeStep(helicopter_M)) {
    /* S-Function (hil_read_encoder_timebase_block): '<S5>/HIL Read Encoder Timebase' */

    /* S-Function Block: helicopter/Helicopter_interface/HIL Read Encoder Timebase (hil_read_encoder_timebase_block) */
    {
      t_error result;
      result = hil_task_read_encoder(helicopter_DW.HILReadEncoderTimebase_Task,
        1, &helicopter_DW.HILReadEncoderTimebase_Buffer[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
      } else {
        rtb_HILReadEncoderTimebase_o1 =
          helicopter_DW.HILReadEncoderTimebase_Buffer[0];
        rtb_HILReadEncoderTimebase_o2 =
          helicopter_DW.HILReadEncoderTimebase_Buffer[1];
        rtb_HILReadEncoderTimebase_o3 =
          helicopter_DW.HILReadEncoderTimebase_Buffer[2];
      }
    }

    /* Gain: '<S5>/Elevation: Count to rad' */
    helicopter_B.ElevationCounttorad = helicopter_P.ElevationCounttorad_Gain *
      rtb_HILReadEncoderTimebase_o3;

    /* Gain: '<S9>/Gain' */
    helicopter_B.Gain = helicopter_P.Gain_Gain *
      helicopter_B.ElevationCounttorad;

    /* Sum: '<Root>/Sum' incorporates:
     *  Constant: '<Root>/elavation_offset [deg]'
     */
    helicopter_B.Sum = helicopter_B.Gain +
      helicopter_P.elavation_offsetdeg_Value;

    /* Gain: '<S5>/Pitch: Count to rad' */
    helicopter_B.PitchCounttorad = helicopter_P.PitchCounttorad_Gain *
      rtb_HILReadEncoderTimebase_o2;

    /* Gain: '<S11>/Gain' */
    helicopter_B.Gain_i = helicopter_P.Gain_Gain_a *
      helicopter_B.PitchCounttorad;
  }

  /* FromWorkspace: '<Root>/     ' */
  {
    real_T *pDataValues = (real_T *) helicopter_DW._PWORK.DataPtr;
    real_T *pTimeValues = (real_T *) helicopter_DW._PWORK.TimePtr;
    int_T currTimeIndex = helicopter_DW._IWORK.PrevIndex;
    real_T t = helicopter_M->Timing.t[0];

    /* Get index */
    if (t <= pTimeValues[0]) {
      currTimeIndex = 0;
    } else if (t >= pTimeValues[140]) {
      currTimeIndex = 139;
    } else {
      if (t < pTimeValues[currTimeIndex]) {
        while (t < pTimeValues[currTimeIndex]) {
          currTimeIndex--;
        }
      } else {
        while (t >= pTimeValues[currTimeIndex + 1]) {
          currTimeIndex++;
        }
      }
    }

    helicopter_DW._IWORK.PrevIndex = currTimeIndex;

    /* Post output */
    {
      real_T t1 = pTimeValues[currTimeIndex];
      real_T t2 = pTimeValues[currTimeIndex + 1];
      if (t1 == t2) {
        if (t < t1) {
          rtb_Derivative = pDataValues[currTimeIndex];
        } else {
          rtb_Derivative = pDataValues[currTimeIndex + 1];
        }
      } else {
        real_T f1 = (t2 - t) / (t2 - t1);
        real_T f2 = 1.0 - f1;
        real_T d1;
        real_T d2;
        int_T TimeIndex= currTimeIndex;
        d1 = pDataValues[TimeIndex];
        d2 = pDataValues[TimeIndex + 1];
        rtb_Derivative = (real_T) rtInterpolate(d1, d2, f1, f2);
        pDataValues += 141;
      }
    }
  }

  /* FromWorkspace: '<Root>/  ' */
  {
    real_T *pDataValues = (real_T *) helicopter_DW._PWORK_k.DataPtr;
    real_T *pTimeValues = (real_T *) helicopter_DW._PWORK_k.TimePtr;
    int_T currTimeIndex = helicopter_DW._IWORK_g.PrevIndex;
    real_T t = helicopter_M->Timing.t[0];

    /* Get index */
    if (t <= pTimeValues[0]) {
      currTimeIndex = 0;
    } else if (t >= pTimeValues[140]) {
      currTimeIndex = 139;
    } else {
      if (t < pTimeValues[currTimeIndex]) {
        while (t < pTimeValues[currTimeIndex]) {
          currTimeIndex--;
        }
      } else {
        while (t >= pTimeValues[currTimeIndex + 1]) {
          currTimeIndex++;
        }
      }
    }

    helicopter_DW._IWORK_g.PrevIndex = currTimeIndex;

    /* Post output */
    {
      real_T t1 = pTimeValues[currTimeIndex];
      real_T t2 = pTimeValues[currTimeIndex + 1];
      if (t1 == t2) {
        if (t < t1) {
          {
            int_T elIdx;
            for (elIdx = 0; elIdx < 4; ++elIdx) {
              (&rtb_Sum4[0])[elIdx] = pDataValues[currTimeIndex];
              pDataValues += 141;
            }
          }
        } else {
          {
            int_T elIdx;
            for (elIdx = 0; elIdx < 4; ++elIdx) {
              (&rtb_Sum4[0])[elIdx] = pDataValues[currTimeIndex + 1];
              pDataValues += 141;
            }
          }
        }
      } else {
        real_T f1 = (t2 - t) / (t2 - t1);
        real_T f2 = 1.0 - f1;
        real_T d1;
        real_T d2;
        int_T TimeIndex= currTimeIndex;

        {
          int_T elIdx;
          for (elIdx = 0; elIdx < 4; ++elIdx) {
            d1 = pDataValues[TimeIndex];
            d2 = pDataValues[TimeIndex + 1];
            (&rtb_Sum4[0])[elIdx] = (real_T) rtInterpolate(d1, d2, f1, f2);
            pDataValues += 141;
          }
        }
      }
    }
  }

  if (rtmIsMajorTimeStep(helicopter_M)) {
    /* Gain: '<S5>/Gain' incorporates:
     *  Gain: '<S5>/Travel: Count to rad'
     */
    helicopter_B.Gain_g = helicopter_P.TravelCounttorad_Gain *
      rtb_HILReadEncoderTimebase_o1 * helicopter_P.Gain_Gain_b;

    /* Gain: '<S14>/Gain' */
    helicopter_B.Gain_p = helicopter_P.Gain_Gain_ar * helicopter_B.Gain_g;

    /* Sum: '<Root>/Sum3' incorporates:
     *  Constant: '<Root>/Travel offset [deg]'
     */
    helicopter_B.Sum3 = helicopter_B.Gain_p + helicopter_P.Traveloffsetdeg_Value;
  }

  /* TransferFcn: '<S5>/Travel: Transfer Fcn' */
  rtb_Backgain = 0.0;
  rtb_Backgain += helicopter_P.TravelTransferFcn_C *
    helicopter_X.TravelTransferFcn_CSTATE;
  rtb_Backgain += helicopter_P.TravelTransferFcn_D * helicopter_B.Gain_g;

  /* Gain: '<S15>/Gain' */
  helicopter_B.Gain_d = helicopter_P.Gain_Gain_l * rtb_Backgain;

  /* TransferFcn: '<S5>/Pitch: Transfer Fcn' */
  rtb_Backgain = 0.0;
  rtb_Backgain += helicopter_P.PitchTransferFcn_C *
    helicopter_X.PitchTransferFcn_CSTATE;
  rtb_Backgain += helicopter_P.PitchTransferFcn_D * helicopter_B.PitchCounttorad;

  /* Gain: '<S12>/Gain' */
  helicopter_B.Gain_b = helicopter_P.Gain_Gain_ae * rtb_Backgain;

  /* TransferFcn: '<S5>/Elevation: Transfer Fcn' */
  rtb_Backgain = 0.0;
  rtb_Backgain += helicopter_P.ElevationTransferFcn_C *
    helicopter_X.ElevationTransferFcn_CSTATE;
  rtb_Backgain += helicopter_P.ElevationTransferFcn_D *
    helicopter_B.ElevationCounttorad;

  /* Gain: '<S10>/Gain' */
  helicopter_B.Gain_dg = helicopter_P.Gain_Gain_n * rtb_Backgain;

  /* Gain: '<S3>/Gain1' */
  helicopter_B.Gain1[0] = helicopter_P.Gain1_Gain * helicopter_B.Sum3;
  helicopter_B.Gain1[1] = helicopter_P.Gain1_Gain * helicopter_B.Gain_d;
  helicopter_B.Gain1[2] = helicopter_P.Gain1_Gain * helicopter_B.Gain_i;
  helicopter_B.Gain1[3] = helicopter_P.Gain1_Gain * helicopter_B.Gain_b;
  helicopter_B.Gain1[4] = helicopter_P.Gain1_Gain * helicopter_B.Sum;
  helicopter_B.Gain1[5] = helicopter_P.Gain1_Gain * helicopter_B.Gain_dg;

  /* Sum: '<S6>/Sum4' */
  rtb_Sum4[0] = helicopter_B.Gain1[0] - rtb_Sum4[0];
  rtb_Sum4[1] = helicopter_B.Gain1[1] - rtb_Sum4[1];
  rtb_Sum4[2] = helicopter_B.Gain1[2] - rtb_Sum4[2];
  rtb_Sum4[3] = helicopter_B.Gain1[3] - rtb_Sum4[3];

  /* Gain: '<S6>/Gain' */
  rtb_Backgain = ((helicopter_P.K[0] * rtb_Sum4[0] + helicopter_P.K[1] *
                   rtb_Sum4[1]) + helicopter_P.K[2] * rtb_Sum4[2]) +
    helicopter_P.K[3] * rtb_Sum4[3];

  /* Sum: '<S6>/Sum5' */
  helicopter_B.Sum5 = rtb_Derivative - rtb_Backgain;
  if (rtmIsMajorTimeStep(helicopter_M)) {
    /* ToFile: '<Root>/To File3' */
    {
      if (!(++helicopter_DW.ToFile3_IWORK.Decimation % 1) &&
          (helicopter_DW.ToFile3_IWORK.Count*2)+1 < 100000000 ) {
        FILE *fp = (FILE *) helicopter_DW.ToFile3_PWORK.FilePtr;
        if (fp != (NULL)) {
          real_T u[2];
          helicopter_DW.ToFile3_IWORK.Decimation = 0;
          u[0] = helicopter_M->Timing.t[1];
          u[1] = helicopter_B.Sum5;
          if (fwrite(u, sizeof(real_T), 2, fp) != 2) {
            rtmSetErrorStatus(helicopter_M,
                              "Error writing to MAT-file p_ref_practical.mat");
            return;
          }

          if (((++helicopter_DW.ToFile3_IWORK.Count)*2)+1 >= 100000000) {
            (void)fprintf(stdout,
                          "*** The ToFile block will stop logging data before\n"
                          "    the simulation has ended, because it has reached\n"
                          "    the maximum number of elements (100000000)\n"
                          "    allowed in MAT-file p_ref_practical.mat.\n");
          }
        }
      }
    }

    /* ToFile: '<S8>/To File' */
    {
      if (!(++helicopter_DW.ToFile_IWORK.Decimation % 1) &&
          (helicopter_DW.ToFile_IWORK.Count*2)+1 < 100000000 ) {
        FILE *fp = (FILE *) helicopter_DW.ToFile_PWORK.FilePtr;
        if (fp != (NULL)) {
          real_T u[2];
          helicopter_DW.ToFile_IWORK.Decimation = 0;
          u[0] = helicopter_M->Timing.t[1];
          u[1] = helicopter_B.Gain1[3];
          if (fwrite(u, sizeof(real_T), 2, fp) != 2) {
            rtmSetErrorStatus(helicopter_M,
                              "Error writing to MAT-file p_dot_practical.mat");
            return;
          }

          if (((++helicopter_DW.ToFile_IWORK.Count)*2)+1 >= 100000000) {
            (void)fprintf(stdout,
                          "*** The ToFile block will stop logging data before\n"
                          "    the simulation has ended, because it has reached\n"
                          "    the maximum number of elements (100000000)\n"
                          "    allowed in MAT-file p_dot_practical.mat.\n");
          }
        }
      }
    }

    /* ToFile: '<S8>/To File1' */
    {
      if (!(++helicopter_DW.ToFile1_IWORK.Decimation % 1) &&
          (helicopter_DW.ToFile1_IWORK.Count*2)+1 < 100000000 ) {
        FILE *fp = (FILE *) helicopter_DW.ToFile1_PWORK.FilePtr;
        if (fp != (NULL)) {
          real_T u[2];
          helicopter_DW.ToFile1_IWORK.Decimation = 0;
          u[0] = helicopter_M->Timing.t[1];
          u[1] = helicopter_B.Gain1[2];
          if (fwrite(u, sizeof(real_T), 2, fp) != 2) {
            rtmSetErrorStatus(helicopter_M,
                              "Error writing to MAT-file p_practical.mat");
            return;
          }

          if (((++helicopter_DW.ToFile1_IWORK.Count)*2)+1 >= 100000000) {
            (void)fprintf(stdout,
                          "*** The ToFile block will stop logging data before\n"
                          "    the simulation has ended, because it has reached\n"
                          "    the maximum number of elements (100000000)\n"
                          "    allowed in MAT-file p_practical.mat.\n");
          }
        }
      }
    }

    /* ToFile: '<S8>/To File2' */
    {
      if (!(++helicopter_DW.ToFile2_IWORK.Decimation % 1) &&
          (helicopter_DW.ToFile2_IWORK.Count*2)+1 < 100000000 ) {
        FILE *fp = (FILE *) helicopter_DW.ToFile2_PWORK.FilePtr;
        if (fp != (NULL)) {
          real_T u[2];
          helicopter_DW.ToFile2_IWORK.Decimation = 0;
          u[0] = helicopter_M->Timing.t[1];
          u[1] = helicopter_B.Gain1[0];
          if (fwrite(u, sizeof(real_T), 2, fp) != 2) {
            rtmSetErrorStatus(helicopter_M,
                              "Error writing to MAT-file lambda_practical.mat");
            return;
          }

          if (((++helicopter_DW.ToFile2_IWORK.Count)*2)+1 >= 100000000) {
            (void)fprintf(stdout,
                          "*** The ToFile block will stop logging data before\n"
                          "    the simulation has ended, because it has reached\n"
                          "    the maximum number of elements (100000000)\n"
                          "    allowed in MAT-file lambda_practical.mat.\n");
          }
        }
      }
    }

    /* ToFile: '<S8>/To File4' */
    {
      if (!(++helicopter_DW.ToFile4_IWORK.Decimation % 1) &&
          (helicopter_DW.ToFile4_IWORK.Count*2)+1 < 100000000 ) {
        FILE *fp = (FILE *) helicopter_DW.ToFile4_PWORK.FilePtr;
        if (fp != (NULL)) {
          real_T u[2];
          helicopter_DW.ToFile4_IWORK.Decimation = 0;
          u[0] = helicopter_M->Timing.t[1];
          u[1] = helicopter_B.Gain1[1];
          if (fwrite(u, sizeof(real_T), 2, fp) != 2) {
            rtmSetErrorStatus(helicopter_M,
                              "Error writing to MAT-file r_practical.mat");
            return;
          }

          if (((++helicopter_DW.ToFile4_IWORK.Count)*2)+1 >= 100000000) {
            (void)fprintf(stdout,
                          "*** The ToFile block will stop logging data before\n"
                          "    the simulation has ended, because it has reached\n"
                          "    the maximum number of elements (100000000)\n"
                          "    allowed in MAT-file r_practical.mat.\n");
          }
        }
      }
    }
  }

  /* Sum: '<Root>/Sum1' incorporates:
   *  Constant: '<Root>/Vd_bias'
   *  Gain: '<S7>/K_pd'
   *  Gain: '<S7>/K_pp'
   *  Sum: '<S7>/Sum2'
   *  Sum: '<S7>/Sum3'
   */
  helicopter_B.Sum1 = ((helicopter_B.Sum5 - helicopter_B.Gain1[2]) *
                       helicopter_P.K_pp - helicopter_P.K_pd *
                       helicopter_B.Gain1[3]) + helicopter_P.Vd_ff;
  if (rtmIsMajorTimeStep(helicopter_M)) {
  }

  /* Integrator: '<S4>/Integrator'
   *
   * Regarding '<S4>/Integrator':
   *  Limited Integrator
   */
  if (helicopter_X.Integrator_CSTATE >= helicopter_P.Integrator_UpperSat ) {
    helicopter_X.Integrator_CSTATE = helicopter_P.Integrator_UpperSat;
  } else if (helicopter_X.Integrator_CSTATE <= (helicopter_P.Integrator_LowerSat)
             ) {
    helicopter_X.Integrator_CSTATE = (helicopter_P.Integrator_LowerSat);
  }

  rtb_Backgain = helicopter_X.Integrator_CSTATE;
  if (rtmIsMajorTimeStep(helicopter_M)) {
    /* Gain: '<S2>/Gain1' incorporates:
     *  Constant: '<Root>/elevation_ref'
     */
    helicopter_B.Gain1_f = helicopter_P.Gain1_Gain_m *
      helicopter_P.elevation_ref_Value;
  }

  /* Sum: '<S4>/Sum' */
  rtb_Derivative = helicopter_B.Gain1_f - helicopter_B.Gain1[4];

  /* Sum: '<Root>/Sum2' incorporates:
   *  Constant: '<Root>/Vs_bias'
   *  Gain: '<S4>/K_ed'
   *  Gain: '<S4>/K_ep'
   *  Sum: '<S4>/Sum1'
   */
  helicopter_B.Sum2 = ((helicopter_P.K_ep * rtb_Derivative + rtb_Backgain) -
                       helicopter_P.K_ed * helicopter_B.Gain1[5]) +
    helicopter_P.Vs_ff;
  if (rtmIsMajorTimeStep(helicopter_M)) {
  }

  /* Gain: '<S1>/Back gain' incorporates:
   *  Sum: '<S1>/Subtract'
   */
  rtb_Backgain = (helicopter_B.Sum2 - helicopter_B.Sum1) *
    helicopter_P.Backgain_Gain;

  /* Gain: '<S4>/K_ei' */
  helicopter_B.K_ei = helicopter_P.K_ei * rtb_Derivative;
  if (rtmIsMajorTimeStep(helicopter_M)) {
  }

  /* Derivative: '<S5>/Derivative' */
  if ((helicopter_DW.TimeStampA >= helicopter_M->Timing.t[0]) &&
      (helicopter_DW.TimeStampB >= helicopter_M->Timing.t[0])) {
    rtb_Derivative = 0.0;
  } else {
    lastTime = helicopter_DW.TimeStampA;
    lastU = &helicopter_DW.LastUAtTimeA;
    if (helicopter_DW.TimeStampA < helicopter_DW.TimeStampB) {
      if (helicopter_DW.TimeStampB < helicopter_M->Timing.t[0]) {
        lastTime = helicopter_DW.TimeStampB;
        lastU = &helicopter_DW.LastUAtTimeB;
      }
    } else {
      if (helicopter_DW.TimeStampA >= helicopter_M->Timing.t[0]) {
        lastTime = helicopter_DW.TimeStampB;
        lastU = &helicopter_DW.LastUAtTimeB;
      }
    }

    rtb_Derivative = (helicopter_B.PitchCounttorad - *lastU) /
      (helicopter_M->Timing.t[0] - lastTime);
  }

  /* End of Derivative: '<S5>/Derivative' */

  /* Gain: '<S13>/Gain' */
  helicopter_B.Gain_l = helicopter_P.Gain_Gain_a1 * rtb_Derivative;
  if (rtmIsMajorTimeStep(helicopter_M)) {
  }

  /* Saturate: '<S5>/Back motor: Saturation' */
  if (rtb_Backgain > helicopter_P.BackmotorSaturation_UpperSat) {
    helicopter_B.BackmotorSaturation = helicopter_P.BackmotorSaturation_UpperSat;
  } else if (rtb_Backgain < helicopter_P.BackmotorSaturation_LowerSat) {
    helicopter_B.BackmotorSaturation = helicopter_P.BackmotorSaturation_LowerSat;
  } else {
    helicopter_B.BackmotorSaturation = rtb_Backgain;
  }

  /* End of Saturate: '<S5>/Back motor: Saturation' */
  if (rtmIsMajorTimeStep(helicopter_M)) {
  }

  /* Gain: '<S1>/Front gain' incorporates:
   *  Sum: '<S1>/Add'
   */
  lastTime = (helicopter_B.Sum1 + helicopter_B.Sum2) *
    helicopter_P.Frontgain_Gain;

  /* Saturate: '<S5>/Front motor: Saturation' */
  if (lastTime > helicopter_P.FrontmotorSaturation_UpperSat) {
    helicopter_B.FrontmotorSaturation =
      helicopter_P.FrontmotorSaturation_UpperSat;
  } else if (lastTime < helicopter_P.FrontmotorSaturation_LowerSat) {
    helicopter_B.FrontmotorSaturation =
      helicopter_P.FrontmotorSaturation_LowerSat;
  } else {
    helicopter_B.FrontmotorSaturation = lastTime;
  }

  /* End of Saturate: '<S5>/Front motor: Saturation' */
  if (rtmIsMajorTimeStep(helicopter_M)) {
    /* S-Function (hil_write_analog_block): '<S5>/HIL Write Analog' */

    /* S-Function Block: helicopter/Helicopter_interface/HIL Write Analog (hil_write_analog_block) */
    {
      t_error result;
      helicopter_DW.HILWriteAnalog_Buffer[0] = helicopter_B.FrontmotorSaturation;
      helicopter_DW.HILWriteAnalog_Buffer[1] = helicopter_B.BackmotorSaturation;
      result = hil_write_analog(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILWriteAnalog_channels, 2,
        &helicopter_DW.HILWriteAnalog_Buffer[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
      }
    }
  }
}

/* Model update function */
void helicopter_update(void)
{
  real_T *lastU;

  /* Update for Derivative: '<S5>/Derivative' */
  if (helicopter_DW.TimeStampA == (rtInf)) {
    helicopter_DW.TimeStampA = helicopter_M->Timing.t[0];
    lastU = &helicopter_DW.LastUAtTimeA;
  } else if (helicopter_DW.TimeStampB == (rtInf)) {
    helicopter_DW.TimeStampB = helicopter_M->Timing.t[0];
    lastU = &helicopter_DW.LastUAtTimeB;
  } else if (helicopter_DW.TimeStampA < helicopter_DW.TimeStampB) {
    helicopter_DW.TimeStampA = helicopter_M->Timing.t[0];
    lastU = &helicopter_DW.LastUAtTimeA;
  } else {
    helicopter_DW.TimeStampB = helicopter_M->Timing.t[0];
    lastU = &helicopter_DW.LastUAtTimeB;
  }

  *lastU = helicopter_B.PitchCounttorad;

  /* End of Update for Derivative: '<S5>/Derivative' */
  if (rtmIsMajorTimeStep(helicopter_M)) {
    rt_ertODEUpdateContinuousStates(&helicopter_M->solverInfo);
  }

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   * Timer of this task consists of two 32 bit unsigned integers.
   * The two integers represent the low bits Timing.clockTick0 and the high bits
   * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
   */
  if (!(++helicopter_M->Timing.clockTick0)) {
    ++helicopter_M->Timing.clockTickH0;
  }

  helicopter_M->Timing.t[0] = rtsiGetSolverStopTime(&helicopter_M->solverInfo);

  {
    /* Update absolute timer for sample time: [0.002s, 0.0s] */
    /* The "clockTick1" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick1"
     * and "Timing.stepSize1". Size of "clockTick1" ensures timer will not
     * overflow during the application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick1 and the high bits
     * Timing.clockTickH1. When the low bit overflows to 0, the high bits increment.
     */
    if (!(++helicopter_M->Timing.clockTick1)) {
      ++helicopter_M->Timing.clockTickH1;
    }

    helicopter_M->Timing.t[1] = helicopter_M->Timing.clockTick1 *
      helicopter_M->Timing.stepSize1 + helicopter_M->Timing.clockTickH1 *
      helicopter_M->Timing.stepSize1 * 4294967296.0;
  }
}

/* Derivatives for root system: '<Root>' */
void helicopter_derivatives(void)
{
  XDot_helicopter_T *_rtXdot;
  _rtXdot = ((XDot_helicopter_T *) helicopter_M->ModelData.derivs);

  /* Derivatives for TransferFcn: '<S5>/Travel: Transfer Fcn' */
  _rtXdot->TravelTransferFcn_CSTATE = 0.0;
  _rtXdot->TravelTransferFcn_CSTATE += helicopter_P.TravelTransferFcn_A *
    helicopter_X.TravelTransferFcn_CSTATE;
  _rtXdot->TravelTransferFcn_CSTATE += helicopter_B.Gain_g;

  /* Derivatives for TransferFcn: '<S5>/Pitch: Transfer Fcn' */
  _rtXdot->PitchTransferFcn_CSTATE = 0.0;
  _rtXdot->PitchTransferFcn_CSTATE += helicopter_P.PitchTransferFcn_A *
    helicopter_X.PitchTransferFcn_CSTATE;
  _rtXdot->PitchTransferFcn_CSTATE += helicopter_B.PitchCounttorad;

  /* Derivatives for TransferFcn: '<S5>/Elevation: Transfer Fcn' */
  _rtXdot->ElevationTransferFcn_CSTATE = 0.0;
  _rtXdot->ElevationTransferFcn_CSTATE += helicopter_P.ElevationTransferFcn_A *
    helicopter_X.ElevationTransferFcn_CSTATE;
  _rtXdot->ElevationTransferFcn_CSTATE += helicopter_B.ElevationCounttorad;

  /* Derivatives for Integrator: '<S4>/Integrator' */
  {
    boolean_T lsat;
    boolean_T usat;
    lsat = ( helicopter_X.Integrator_CSTATE <= (helicopter_P.Integrator_LowerSat)
            );
    usat = ( helicopter_X.Integrator_CSTATE >= helicopter_P.Integrator_UpperSat );
    if ((!lsat && !usat) ||
        (lsat && (helicopter_B.K_ei > 0)) ||
        (usat && (helicopter_B.K_ei < 0)) ) {
      ((XDot_helicopter_T *) helicopter_M->ModelData.derivs)->Integrator_CSTATE =
        helicopter_B.K_ei;
    } else {
      /* in saturation */
      ((XDot_helicopter_T *) helicopter_M->ModelData.derivs)->Integrator_CSTATE =
        0.0;
    }
  }
}

/* Model initialize function */
void helicopter_initialize(void)
{
  /* Start for S-Function (hil_initialize_block): '<Root>/HIL Initialize' */

  /* S-Function Block: helicopter/HIL Initialize (hil_initialize_block) */
  {
    t_int result;
    t_boolean is_switching;
    result = hil_open("q8_usb", "0", &helicopter_DW.HILInitialize_Card);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(helicopter_M, _rt_error_message);
      return;
    }

    is_switching = false;
    result = hil_set_card_specific_options(helicopter_DW.HILInitialize_Card,
      "update_rate=normal;decimation=1", 32);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(helicopter_M, _rt_error_message);
      return;
    }

    result = hil_watchdog_clear(helicopter_DW.HILInitialize_Card);
    if (result < 0 && result != -QERR_HIL_WATCHDOG_CLEAR) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(helicopter_M, _rt_error_message);
      return;
    }

    if ((helicopter_P.HILInitialize_set_analog_input_ && !is_switching) ||
        (helicopter_P.HILInitialize_set_analog_inpu_m && is_switching)) {
      {
        int_T i1;
        real_T *dw_AIMinimums = &helicopter_DW.HILInitialize_AIMinimums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AIMinimums[i1] = helicopter_P.HILInitialize_analog_input_mini;
        }
      }

      {
        int_T i1;
        real_T *dw_AIMaximums = &helicopter_DW.HILInitialize_AIMaximums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AIMaximums[i1] = helicopter_P.HILInitialize_analog_input_maxi;
        }
      }

      result = hil_set_analog_input_ranges(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILInitialize_analog_input_chan, 8U,
        &helicopter_DW.HILInitialize_AIMinimums[0],
        &helicopter_DW.HILInitialize_AIMaximums[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_P.HILInitialize_set_analog_output && !is_switching) ||
        (helicopter_P.HILInitialize_set_analog_outp_b && is_switching)) {
      {
        int_T i1;
        real_T *dw_AOMinimums = &helicopter_DW.HILInitialize_AOMinimums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOMinimums[i1] = helicopter_P.HILInitialize_analog_output_min;
        }
      }

      {
        int_T i1;
        real_T *dw_AOMaximums = &helicopter_DW.HILInitialize_AOMaximums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOMaximums[i1] = helicopter_P.HILInitialize_analog_output_max;
        }
      }

      result = hil_set_analog_output_ranges(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILInitialize_analog_output_cha, 8U,
        &helicopter_DW.HILInitialize_AOMinimums[0],
        &helicopter_DW.HILInitialize_AOMaximums[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_P.HILInitialize_set_analog_outp_e && !is_switching) ||
        (helicopter_P.HILInitialize_set_analog_outp_j && is_switching)) {
      {
        int_T i1;
        real_T *dw_AOVoltages = &helicopter_DW.HILInitialize_AOVoltages[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOVoltages[i1] = helicopter_P.HILInitialize_initial_analog_ou;
        }
      }

      result = hil_write_analog(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILInitialize_analog_output_cha, 8U,
        &helicopter_DW.HILInitialize_AOVoltages[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }
    }

    if (helicopter_P.HILInitialize_set_analog_outp_p) {
      {
        int_T i1;
        real_T *dw_AOVoltages = &helicopter_DW.HILInitialize_AOVoltages[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOVoltages[i1] = helicopter_P.HILInitialize_watchdog_analog_o;
        }
      }

      result = hil_watchdog_set_analog_expiration_state
        (helicopter_DW.HILInitialize_Card,
         helicopter_P.HILInitialize_analog_output_cha, 8U,
         &helicopter_DW.HILInitialize_AOVoltages[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_P.HILInitialize_set_encoder_param && !is_switching) ||
        (helicopter_P.HILInitialize_set_encoder_par_m && is_switching)) {
      {
        int_T i1;
        int32_T *dw_QuadratureModes =
          &helicopter_DW.HILInitialize_QuadratureModes[0];
        for (i1=0; i1 < 8; i1++) {
          dw_QuadratureModes[i1] = helicopter_P.HILInitialize_quadrature;
        }
      }

      result = hil_set_encoder_quadrature_mode(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILInitialize_encoder_channels, 8U,
        (t_encoder_quadrature_mode *)
        &helicopter_DW.HILInitialize_QuadratureModes[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_P.HILInitialize_set_encoder_count && !is_switching) ||
        (helicopter_P.HILInitialize_set_encoder_cou_k && is_switching)) {
      {
        int_T i1;
        int32_T *dw_InitialEICounts =
          &helicopter_DW.HILInitialize_InitialEICounts[0];
        for (i1=0; i1 < 8; i1++) {
          dw_InitialEICounts[i1] = helicopter_P.HILInitialize_initial_encoder_c;
        }
      }

      result = hil_set_encoder_counts(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILInitialize_encoder_channels, 8U,
        &helicopter_DW.HILInitialize_InitialEICounts[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_P.HILInitialize_set_pwm_params_at && !is_switching) ||
        (helicopter_P.HILInitialize_set_pwm_params__f && is_switching)) {
      uint32_T num_duty_cycle_modes = 0;
      uint32_T num_frequency_modes = 0;

      {
        int_T i1;
        int32_T *dw_POModeValues = &helicopter_DW.HILInitialize_POModeValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POModeValues[i1] = helicopter_P.HILInitialize_pwm_modes;
        }
      }

      result = hil_set_pwm_mode(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILInitialize_pwm_channels, 8U, (t_pwm_mode *)
        &helicopter_DW.HILInitialize_POModeValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }

      {
        int_T i1;
        const uint32_T *p_HILInitialize_pwm_channels =
          helicopter_P.HILInitialize_pwm_channels;
        int32_T *dw_POModeValues = &helicopter_DW.HILInitialize_POModeValues[0];
        for (i1=0; i1 < 8; i1++) {
          if (dw_POModeValues[i1] == PWM_DUTY_CYCLE_MODE || dw_POModeValues[i1] ==
              PWM_ONE_SHOT_MODE || dw_POModeValues[i1] == PWM_TIME_MODE) {
            helicopter_DW.HILInitialize_POSortedChans[num_duty_cycle_modes] =
              p_HILInitialize_pwm_channels[i1];
            helicopter_DW.HILInitialize_POSortedFreqs[num_duty_cycle_modes] =
              helicopter_P.HILInitialize_pwm_frequency;
            num_duty_cycle_modes++;
          } else {
            helicopter_DW.HILInitialize_POSortedChans[7U - num_frequency_modes] =
              p_HILInitialize_pwm_channels[i1];
            helicopter_DW.HILInitialize_POSortedFreqs[7U - num_frequency_modes] =
              helicopter_P.HILInitialize_pwm_frequency;
            num_frequency_modes++;
          }
        }
      }

      if (num_duty_cycle_modes > 0) {
        result = hil_set_pwm_frequency(helicopter_DW.HILInitialize_Card,
          &helicopter_DW.HILInitialize_POSortedChans[0], num_duty_cycle_modes,
          &helicopter_DW.HILInitialize_POSortedFreqs[0]);
        if (result < 0) {
          msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
            (_rt_error_message));
          rtmSetErrorStatus(helicopter_M, _rt_error_message);
          return;
        }
      }

      if (num_frequency_modes > 0) {
        result = hil_set_pwm_duty_cycle(helicopter_DW.HILInitialize_Card,
          &helicopter_DW.HILInitialize_POSortedChans[num_duty_cycle_modes],
          num_frequency_modes,
          &helicopter_DW.HILInitialize_POSortedFreqs[num_duty_cycle_modes]);
        if (result < 0) {
          msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
            (_rt_error_message));
          rtmSetErrorStatus(helicopter_M, _rt_error_message);
          return;
        }
      }

      {
        int_T i1;
        int32_T *dw_POModeValues = &helicopter_DW.HILInitialize_POModeValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POModeValues[i1] = helicopter_P.HILInitialize_pwm_configuration;
        }
      }

      {
        int_T i1;
        int32_T *dw_POAlignValues = &helicopter_DW.HILInitialize_POAlignValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POAlignValues[i1] = helicopter_P.HILInitialize_pwm_alignment;
        }
      }

      {
        int_T i1;
        int32_T *dw_POPolarityVals =
          &helicopter_DW.HILInitialize_POPolarityVals[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POPolarityVals[i1] = helicopter_P.HILInitialize_pwm_polarity;
        }
      }

      result = hil_set_pwm_configuration(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILInitialize_pwm_channels, 8U,
        (t_pwm_configuration *) &helicopter_DW.HILInitialize_POModeValues[0],
        (t_pwm_alignment *) &helicopter_DW.HILInitialize_POAlignValues[0],
        (t_pwm_polarity *) &helicopter_DW.HILInitialize_POPolarityVals[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }

      {
        int_T i1;
        real_T *dw_POSortedFreqs = &helicopter_DW.HILInitialize_POSortedFreqs[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POSortedFreqs[i1] = helicopter_P.HILInitialize_pwm_leading_deadb;
        }
      }

      {
        int_T i1;
        real_T *dw_POValues = &helicopter_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = helicopter_P.HILInitialize_pwm_trailing_dead;
        }
      }

      result = hil_set_pwm_deadband(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILInitialize_pwm_channels, 8U,
        &helicopter_DW.HILInitialize_POSortedFreqs[0],
        &helicopter_DW.HILInitialize_POValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_P.HILInitialize_set_pwm_outputs_a && !is_switching) ||
        (helicopter_P.HILInitialize_set_pwm_outputs_g && is_switching)) {
      {
        int_T i1;
        real_T *dw_POValues = &helicopter_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = helicopter_P.HILInitialize_initial_pwm_outpu;
        }
      }

      result = hil_write_pwm(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILInitialize_pwm_channels, 8U,
        &helicopter_DW.HILInitialize_POValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }
    }

    if (helicopter_P.HILInitialize_set_pwm_outputs_o) {
      {
        int_T i1;
        real_T *dw_POValues = &helicopter_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = helicopter_P.HILInitialize_watchdog_pwm_outp;
        }
      }

      result = hil_watchdog_set_pwm_expiration_state
        (helicopter_DW.HILInitialize_Card,
         helicopter_P.HILInitialize_pwm_channels, 8U,
         &helicopter_DW.HILInitialize_POValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }
    }
  }

  /* Start for S-Function (hil_read_encoder_timebase_block): '<S5>/HIL Read Encoder Timebase' */

  /* S-Function Block: helicopter/Helicopter_interface/HIL Read Encoder Timebase (hil_read_encoder_timebase_block) */
  {
    t_error result;
    result = hil_task_create_encoder_reader(helicopter_DW.HILInitialize_Card,
      helicopter_P.HILReadEncoderTimebase_samples_,
      helicopter_P.HILReadEncoderTimebase_channels, 3,
      &helicopter_DW.HILReadEncoderTimebase_Task);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(helicopter_M, _rt_error_message);
    }
  }

  /* Start for FromWorkspace: '<Root>/     ' */
  {
    static real_T pTimeValues0[] = { 0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75,
      2.0, 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 3.75, 4.0, 4.25, 4.5, 4.75, 5.0,
      5.25, 5.5, 5.75, 6.0, 6.25, 6.5, 6.75, 7.0, 7.25, 7.5, 7.75, 8.0, 8.25,
      8.5, 8.75, 9.0, 9.25, 9.5, 9.75, 10.0, 10.25, 10.5, 10.75, 11.0, 11.25,
      11.5, 11.75, 12.0, 12.25, 12.5, 12.75, 13.0, 13.25, 13.5, 13.75, 14.0,
      14.25, 14.5, 14.75, 15.0, 15.25, 15.5, 15.75, 16.0, 16.25, 16.5, 16.75,
      17.0, 17.25, 17.5, 17.75, 18.0, 18.25, 18.5, 18.75, 19.0, 19.25, 19.5,
      19.75, 20.0, 20.25, 20.5, 20.75, 21.0, 21.25, 21.5, 21.75, 22.0, 22.25,
      22.5, 22.75, 23.0, 23.25, 23.5, 23.75, 24.0, 24.25, 24.5, 24.75, 25.0,
      25.25, 25.5, 25.75, 26.0, 26.25, 26.5, 26.75, 27.0, 27.25, 27.5, 27.75,
      28.0, 28.25, 28.5, 28.75, 29.0, 29.25, 29.5, 29.75, 30.0, 30.25, 30.5,
      30.75, 31.0, 31.25, 31.5, 31.75, 32.0, 32.25, 32.5, 32.75, 33.0, 33.25,
      33.5, 33.75, 34.0, 34.25, 34.5, 34.75, 35.0 } ;

    static real_T pDataValues0[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.52359877557065593,
      0.5235987755648287, 0.52359877555398415, 0.52359877554595047,
      0.52359877553554213, 0.52359877551865264, 0.52359877549472145,
      0.52359877543879152, 0.5235987753775585, 0.52359877516608888,
      0.52359877440812641, 0.38856151693560581, 0.10947154576688355,
      -0.11008507929484017, -0.27695896968853234, -0.39795513806507837,
      -0.47968511189524515, -0.52317212167451765, -0.52359758885893459,
      -0.5235976653329798, -0.52352227216915193, -0.50347393523708073,
      -0.4650060754743196, -0.42080544680465137, -0.37350683555807923,
      -0.32525430068660588, -0.27774421552118844, -0.23227259445252357,
      -0.18978425234113142, -0.15092186790208059, -0.11607345081518927,
      -0.085417096181012064, -0.058962244764331095, -0.0365869517011008,
      -0.018070901997364271, -0.0031241017677711906, 0.0085886759705854,
      0.017427498417380186, 0.023760658261973315, 0.027951507047099831,
      0.030348373830361489, 0.031277192136662961, 0.031036458786475474,
      0.02989416025114857, 0.028086322790823667, 0.025816869231145779,
      0.0232584956740853, 0.020554313921849143, 0.017820038464696857,
      0.015146529393943732, 0.012602533674664745, 0.010237496212285624,
      0.0080843386427272013, 0.0061621275094994018, 0.0044785743479844953,
      0.0030323281761881456, 0.0018150360803460158, 0.00081316013712926329,
      9.5490327520532426E-6, -0.00061522934582098048, -0.0010817821136728932,
      -0.0014109958957998041, -0.0016233323509942602, -0.0017382938394307032,
      -0.0017740377727177082, -0.0017471183870815471, -0.0016723354905480304,
      -0.0015626709929298832, -0.0014292955945502518, -0.001281629768762499,
      -0.0011274450275163893, -0.00097299333024378665, -0.00082315432293468365,
      -0.00068159183027797338, -0.00055091263614977107, -0.00043282205481778689,
      -0.00032827210472720082, -0.00023759924414869241, -0.0001606496150493149,
      -9.68905749236731E-5, -4.5507986359221171E-5, -5.4892939661724852E-6,
      2.430713674672914E-5, 4.5095596936944185E-5, 5.8118439066833538E-5,
      6.4612010149800824E-5, 6.578164129010133E-5, 6.2784055224599484E-5,
      5.6715362742299585E-5, 4.8602564527755183E-5, 3.9396115260259264E-5,
      2.99606084470931E-5, 2.1060013408334364E-5, 1.3333279773730968E-5,
      7.2559901527849331E-6, 3.0853337202106137E-6, 7.9196522271045264E-7,
      2.6461258545282805E-13, 2.72874806029043E-13, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0 } ;

    helicopter_DW._PWORK.TimePtr = (void *) pTimeValues0;
    helicopter_DW._PWORK.DataPtr = (void *) pDataValues0;
    helicopter_DW._IWORK.PrevIndex = 0;
  }

  /* Start for FromWorkspace: '<Root>/  ' */
  {
    static real_T pTimeValues0[] = { 0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75,
      2.0, 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 3.75, 4.0, 4.25, 4.5, 4.75, 5.0,
      5.25, 5.5, 5.75, 6.0, 6.25, 6.5, 6.75, 7.0, 7.25, 7.5, 7.75, 8.0, 8.25,
      8.5, 8.75, 9.0, 9.25, 9.5, 9.75, 10.0, 10.25, 10.5, 10.75, 11.0, 11.25,
      11.5, 11.75, 12.0, 12.25, 12.5, 12.75, 13.0, 13.25, 13.5, 13.75, 14.0,
      14.25, 14.5, 14.75, 15.0, 15.25, 15.5, 15.75, 16.0, 16.25, 16.5, 16.75,
      17.0, 17.25, 17.5, 17.75, 18.0, 18.25, 18.5, 18.75, 19.0, 19.25, 19.5,
      19.75, 20.0, 20.25, 20.5, 20.75, 21.0, 21.25, 21.5, 21.75, 22.0, 22.25,
      22.5, 22.75, 23.0, 23.25, 23.5, 23.75, 24.0, 24.25, 24.5, 24.75, 25.0,
      25.25, 25.5, 25.75, 26.0, 26.25, 26.5, 26.75, 27.0, 27.25, 27.5, 27.75,
      28.0, 28.25, 28.5, 28.75, 29.0, 29.25, 29.5, 29.75, 30.0, 30.25, 30.5,
      30.75, 31.0, 31.25, 31.5, 31.75, 32.0, 32.25, 32.5, 32.75, 33.0, 33.25,
      33.5, 33.75, 34.0, 34.25, 34.5, 34.75, 35.0 } ;

    static real_T pDataValues0[] = { 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1378421413513737, 3.1262155534475076,
      3.1033093000209022, 3.0666274151846253, 3.0144539223915232,
      2.9456562771206443, 2.8595077633046264, 2.755551587987362,
      2.6335051105279459, 2.4931956060921214, 2.3345185761615084,
      2.158378586849047, 1.9678013793910309, 1.7674143111840395,
      1.5625874980938679, 1.3586949815519549, 1.1606091748584675,
      0.97237558041863814, 0.79697339345539708, 0.63645596478416533,
      0.49218023370945158, 0.36487408959506562, 0.25465219321775517,
      0.1611038779743855, 0.083403928095003924, 0.02042320235065569,
      -0.029171039238896471, -0.066828956411080581, -0.0940469129338131,
      -0.11231060497587364, -0.12305126577742861, -0.12761341876914012,
      -0.12723258646661925, -0.12302139842084622, -0.11596263112850516,
      -0.10690783149026066, -0.09658030925634449, -0.085581424264179945,
      -0.0743992352473636, -0.063418714305358739, -0.052932861619315777,
      -0.043154176408169383, -0.034226050896825407, -0.026233753290835146,
      -0.019214753017630842, -0.013168216831196775, -0.0080635681554018668,
      -0.0038480548927893846, -0.00045331367007892556, 0.0021990479275836531,
      0.0041928041520926081, 0.0056133336218349471, 0.0065446340674039927,
      0.0070670446640132955, 0.0072555883992256529, 0.0071788473489172791,
      0.0068982867461100305, 0.0064679486571853654, 0.0059344423489134024,
      0.0053371655463982405, 0.004708698338541371, 0.0040753191533885432,
      0.0034575997404622427, 0.0028710432632298648, 0.0023267362793077079,
      0.0018319914728920578, 0.0013909634467172433, 0.0010052246554293513,
      0.000674292670405935, 0.00039610342959854567, 0.00016742798170867995,
      -1.5767470818722212E-5, -0.00015801663695189132, -0.00026410037795433907,
      -0.00033884224288085904, -0.000386947802174225, -0.00041288294587281139,
      -0.00042078620319196069, -0.00041441021093319131, -0.00039708766735298062,
      -0.00037171741885080656, -0.00034076670716504717, -0.00030628602753693884,
      -0.00026993349082189358, -0.00023300602612681624, -0.00019647519006521163,
      -0.00016102575186731626, -0.00012709559024953545, -9.4915759362575988E-5,
      -6.4549848886681266E-5, -3.5931968247110336E-5, -8.90281586304271E-6,
      1.6756662629216583E-5, 4.1294584513793962E-5, 6.4961146184283784E-5,
      8.7986382401692987E-5, 0.00011056407427959993, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.015002048902967587,
      -0.046506351610164379, -0.091625013701120431, -0.14672753933980565,
      -0.20869397116710756, -0.27519058107821581, -0.34459405525876891,
      -0.41582470126375731, -0.48818590983236315, -0.56123801773799742,
      -0.63470811971715169, -0.70455995724454523, -0.762308829826763,
      -0.80154827282266483, -0.81930725235538615, -0.81557006616235084,
      -0.79234322676864855, -0.752934377754017, -0.70160874784766314,
      -0.6420697146796267, -0.57710292429355414, -0.509224576452243,
      -0.44088758550394086, -0.3741932609681779, -0.31079979951222542,
      -0.25192290297209208, -0.19837696635290777, -0.15063166868343558,
      -0.10887182608562922, -0.07305476816294125, -0.042962643200919047,
      -0.018248611961545202, 0.0015233292153843268, 0.016844752188393059,
      0.028235069174665117, 0.036219198558278848, 0.041310088940965564,
      0.043995539973959025, 0.044728756072566281, 0.043922083773320256,
      0.041943410749472741, 0.039114740849886448, 0.035712502050676762,
      0.031969190429261905, 0.028076001098118083, 0.024186144751037142,
      0.020418594708480495, 0.016862053055750795, 0.013578964896142702,
      0.01060944639595118, 0.0079750249033366822, 0.00568211788427022,
      0.0037252017875770492, 0.0020896423917380765, 0.00075417494615029638,
      -0.0003069641959326281, -0.0011222424059281282, -0.0017213523503977956,
      -0.0021340252277869893, -0.0023891072047597822, -0.0025138688261266123,
      -0.0025335167353104462, -0.0024708776464043374, -0.0023462259036286463,
      -0.002177227930387762, -0.001978979220361735, -0.0017641120993983925,
      -0.0015429551598507027, -0.0013237279347928, -0.0011127569579286922,
      -0.00091470178625859769, -0.00073278180480874347, -0.00056899665923181139,
      -0.00042433495870892595, -0.00029896745440521444, -0.00019242223187259882,
      -0.00010374056949348043, -3.1613023975732068E-5, 2.5503974335942497E-5,
      6.9290179621708169E-5, 0.00010148099930956141, 0.00012380285204390272,
      0.00013792272381329826, 0.00014541015216104611, 0.00014770986408117455,
      0.0001461233495472836, 0.00014179775809244666, 0.00013572065177198836,
      0.00012871932884870295, 0.00012146364720444401, 0.00011447152785914884,
      0.00010811661483713562, 0.00010263791926990229, 9.8151692839174652E-5,
      9.4666251982824409E-5, 9.21009501705019E-5, 9.0310772812492908E-5,
      8.91175814780103E-5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.10602875205327918, 0.22266037931040097, 0.31888147179500592,
      0.38944360628116514, 0.43795507373723969, 0.46997264225343832,
      0.49051724871094826, 0.50343100132673324, 0.51142138573830032,
      0.51630439838192654, 0.51925862085192886, 0.49368610960604919,
      0.40814697577673953, 0.27732939664137851, 0.12551368477112484,
      -0.026413004693958044, -0.16415842996540628, -0.27852669368111443,
      -0.36274994972862468, -0.42079914709287847, -0.45916046212351069,
      -0.47973823822577488, -0.48297975253954006, -0.47137001370813914,
      -0.44804077413625321, -0.41611941829220411, -0.37844222958824858,
      -0.3374455288903177, -0.29514261842798339, -0.25314128603163194,
      -0.21267964635040595, -0.17466933393753284, -0.13974052887506536,
      -0.10828596596594113, -0.080502410232294222, -0.056428777158800125,
      -0.035980468895367587, -0.018979742264847818, -0.00518209135243431,
      0.0057012517244764407, 0.013984505231683665, 0.019991958515872158,
      0.02404572443913635, 0.026456296882856244, 0.027515575293444915,
      0.027492019035032414, 0.026627604786270843, 0.025136278075975071,
      0.023203613224826759, 0.020987422631600583, 0.018619084963370156,
      0.016205391096635834, 0.013830735580032203, 0.011559509151976713,
      0.0094385738596815035, 0.0074997261828547972, 0.00576207501485669,
      0.0042342802737974578, 0.0029166142883153461, 0.0018028219917944012,
      0.00088176749073968813, 0.00013886391839639024, -0.00044270915678906411,
      -0.000880990910267596, -0.0011944131303409996, -0.0014011461664031652,
      -0.0015185987479513054, -0.0015630527834777797, -0.0015494142984667141,
      -0.0014910622894857631, -0.0013997783112202271, -0.0012857409491471732,
      -0.0011575708553120453, -0.0010224136493575194, -0.0008860496393817063,
      -0.00075302094048982921, -0.000626768119833104, -0.00050976994425835139,
      -0.00040368112948215885, -0.00030946417571426721, -0.00022751241885508319,
      -0.00015776232961517561, -9.9793860681817833E-5, -5.29182836616778E-5,
      -1.62534852605619E-5, 1.1212879076374116E-5, 3.0571628636577021E-5,
      4.295066676261295E-5, 4.9482676731625366E-5, 5.1280387025967905E-5,
      4.9417629357450525E-5, 4.4914098428376639E-5, 3.8721328105773227E-5,
      3.1706935295947141E-5, 2.4633765079484398E-5, 1.8130573738751479E-5,
      1.2652290133681462E-5, 8.4330206446613365E-6, 5.4490049989459753E-6, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.42411500821730735,
      0.466526509033788, 0.38488436994372061, 0.28224853794993793,
      0.19404586982959893, 0.12807027407009544, 0.08217842583534056,
      0.051655010468440857, 0.031961537651569116, 0.019532050579805751,
      0.011816889885310468, -0.10229004497821796, -0.34215653531193779,
      -0.52327031653614342, -0.6072628474757138, -0.6077067578550307,
      -0.550981701080492, -0.45747305485753176, -0.33689302418474038,
      -0.23219678945171429, -0.15344526011722778, -0.082311104403755858,
      -0.012966057249760023, 0.046438955330904787, 0.093316958292844532,
      0.12768542338149713, 0.150708754821123, 0.16398680279702435,
      0.16921164185463808, 0.16800532959070669, 0.16184655873020468,
      0.15204124965679333, 0.13971522025517077, 0.1258182516417978,
      0.11113422293988845, 0.096294532299277286, 0.081793233059031009,
      0.068002906527379933, 0.0551906036549549, 0.043533372312943866,
      0.033133014034129764, 0.024029813142054834, 0.01621506369835763,
      0.0096422897801804377, 0.0042371136476555488, -9.4225028349142085E-5,
      -0.0034576569897454257, -0.0059653068358822168, -0.0077306593992923708,
      -0.0088647623676038415, -0.00947335066762084, -0.0096547754616364244,
      -0.0094986220611136558, -0.0090849057069210971, -0.0084837411638799768,
      -0.0077553907020059595, -0.0069506046666915622, -0.00611117895893606,
      -0.0052706639366275829, -0.0044551691807829148, -0.003684217998917987,
      -0.0029716142840723265, -0.0023262922954409523, -0.0017531270086132624,
      -0.0012536888749927495, -0.00082693213894779658, -0.00046981032089169606,
      -0.00017781613680503271, 5.45539453451287E-5, 0.00023340804122466883,
      0.00036513591836300863, 0.00045614945359308035, 0.00051268038064137677,
      0.00054062882911896839, 0.00054545604520411762, 0.00053211480086837377,
      0.00050501128792776538, 0.00046799270759987591, 0.00042435526440563542,
      0.00037686782037243158, 0.00032780703273760131, 0.00027900036226049553,
      0.00023187388103429617, 0.00018750231338142526, 0.00014665919890532875,
      0.00010986546264860918, 7.7435003541676751E-5, 4.9516157805008832E-5,
      2.6128045176914762E-5, 7.1908464782352872E-6, -7.4510253732043958E-6,
      -1.8014118415430426E-5, -2.4771075989548527E-5, -2.8057565938439235E-5,
      -2.8292675564985839E-5, -2.6012760062066559E-5, -2.1913129119414943E-5,
      -1.6877072655215389E-5, -1.1936057281996321E-5, -8.0243524503753151E-6,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0 } ;

    helicopter_DW._PWORK_k.TimePtr = (void *) pTimeValues0;
    helicopter_DW._PWORK_k.DataPtr = (void *) pDataValues0;
    helicopter_DW._IWORK_g.PrevIndex = 0;
  }

  /* Start for ToFile: '<Root>/To File3' */
  {
    char fileName[509] = "p_ref_practical.mat";
    FILE *fp = (NULL);
    if ((fp = fopen(fileName, "wb")) == (NULL)) {
      rtmSetErrorStatus(helicopter_M,
                        "Error creating .mat file p_ref_practical.mat");
      return;
    }

    if (rt_WriteMat4FileHeader(fp,2,0,"p_ref")) {
      rtmSetErrorStatus(helicopter_M,
                        "Error writing mat file header to file p_ref_practical.mat");
      return;
    }

    helicopter_DW.ToFile3_IWORK.Count = 0;
    helicopter_DW.ToFile3_IWORK.Decimation = -1;
    helicopter_DW.ToFile3_PWORK.FilePtr = fp;
  }

  /* Start for ToFile: '<S8>/To File' */
  {
    char fileName[509] = "p_dot_practical.mat";
    FILE *fp = (NULL);
    if ((fp = fopen(fileName, "wb")) == (NULL)) {
      rtmSetErrorStatus(helicopter_M,
                        "Error creating .mat file p_dot_practical.mat");
      return;
    }

    if (rt_WriteMat4FileHeader(fp,2,0,"p_dot")) {
      rtmSetErrorStatus(helicopter_M,
                        "Error writing mat file header to file p_dot_practical.mat");
      return;
    }

    helicopter_DW.ToFile_IWORK.Count = 0;
    helicopter_DW.ToFile_IWORK.Decimation = -1;
    helicopter_DW.ToFile_PWORK.FilePtr = fp;
  }

  /* Start for ToFile: '<S8>/To File1' */
  {
    char fileName[509] = "p_practical.mat";
    FILE *fp = (NULL);
    if ((fp = fopen(fileName, "wb")) == (NULL)) {
      rtmSetErrorStatus(helicopter_M, "Error creating .mat file p_practical.mat");
      return;
    }

    if (rt_WriteMat4FileHeader(fp,2,0,"p")) {
      rtmSetErrorStatus(helicopter_M,
                        "Error writing mat file header to file p_practical.mat");
      return;
    }

    helicopter_DW.ToFile1_IWORK.Count = 0;
    helicopter_DW.ToFile1_IWORK.Decimation = -1;
    helicopter_DW.ToFile1_PWORK.FilePtr = fp;
  }

  /* Start for ToFile: '<S8>/To File2' */
  {
    char fileName[509] = "lambda_practical.mat";
    FILE *fp = (NULL);
    if ((fp = fopen(fileName, "wb")) == (NULL)) {
      rtmSetErrorStatus(helicopter_M,
                        "Error creating .mat file lambda_practical.mat");
      return;
    }

    if (rt_WriteMat4FileHeader(fp,2,0,"lambda")) {
      rtmSetErrorStatus(helicopter_M,
                        "Error writing mat file header to file lambda_practical.mat");
      return;
    }

    helicopter_DW.ToFile2_IWORK.Count = 0;
    helicopter_DW.ToFile2_IWORK.Decimation = -1;
    helicopter_DW.ToFile2_PWORK.FilePtr = fp;
  }

  /* Start for ToFile: '<S8>/To File4' */
  {
    char fileName[509] = "r_practical.mat";
    FILE *fp = (NULL);
    if ((fp = fopen(fileName, "wb")) == (NULL)) {
      rtmSetErrorStatus(helicopter_M, "Error creating .mat file r_practical.mat");
      return;
    }

    if (rt_WriteMat4FileHeader(fp,2,0,"r")) {
      rtmSetErrorStatus(helicopter_M,
                        "Error writing mat file header to file r_practical.mat");
      return;
    }

    helicopter_DW.ToFile4_IWORK.Count = 0;
    helicopter_DW.ToFile4_IWORK.Decimation = -1;
    helicopter_DW.ToFile4_PWORK.FilePtr = fp;
  }

  /* InitializeConditions for TransferFcn: '<S5>/Travel: Transfer Fcn' */
  helicopter_X.TravelTransferFcn_CSTATE = 0.0;

  /* InitializeConditions for TransferFcn: '<S5>/Pitch: Transfer Fcn' */
  helicopter_X.PitchTransferFcn_CSTATE = 0.0;

  /* InitializeConditions for TransferFcn: '<S5>/Elevation: Transfer Fcn' */
  helicopter_X.ElevationTransferFcn_CSTATE = 0.0;

  /* InitializeConditions for Integrator: '<S4>/Integrator' */
  helicopter_X.Integrator_CSTATE = helicopter_P.Integrator_IC;

  /* InitializeConditions for Derivative: '<S5>/Derivative' */
  helicopter_DW.TimeStampA = (rtInf);
  helicopter_DW.TimeStampB = (rtInf);
}

/* Model terminate function */
void helicopter_terminate(void)
{
  /* Terminate for S-Function (hil_initialize_block): '<Root>/HIL Initialize' */

  /* S-Function Block: helicopter/HIL Initialize (hil_initialize_block) */
  {
    t_boolean is_switching;
    t_int result;
    t_uint32 num_final_analog_outputs = 0;
    t_uint32 num_final_pwm_outputs = 0;
    hil_task_stop_all(helicopter_DW.HILInitialize_Card);
    hil_monitor_stop_all(helicopter_DW.HILInitialize_Card);
    is_switching = false;
    if ((helicopter_P.HILInitialize_set_analog_out_ex && !is_switching) ||
        (helicopter_P.HILInitialize_set_analog_outp_c && is_switching)) {
      {
        int_T i1;
        real_T *dw_AOVoltages = &helicopter_DW.HILInitialize_AOVoltages[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOVoltages[i1] = helicopter_P.HILInitialize_final_analog_outp;
        }
      }

      num_final_analog_outputs = 8U;
    }

    if ((helicopter_P.HILInitialize_set_pwm_output_ap && !is_switching) ||
        (helicopter_P.HILInitialize_set_pwm_outputs_p && is_switching)) {
      {
        int_T i1;
        real_T *dw_POValues = &helicopter_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = helicopter_P.HILInitialize_final_pwm_outputs;
        }
      }

      num_final_pwm_outputs = 8U;
    }

    if (0
        || num_final_analog_outputs > 0
        || num_final_pwm_outputs > 0
        ) {
      /* Attempt to write the final outputs atomically (due to firmware issue in old Q2-USB). Otherwise write channels individually */
      result = hil_write(helicopter_DW.HILInitialize_Card
                         , helicopter_P.HILInitialize_analog_output_cha,
                         num_final_analog_outputs
                         , helicopter_P.HILInitialize_pwm_channels,
                         num_final_pwm_outputs
                         , NULL, 0
                         , NULL, 0
                         , &helicopter_DW.HILInitialize_AOVoltages[0]
                         , &helicopter_DW.HILInitialize_POValues[0]
                         , (t_boolean *) NULL
                         , NULL
                         );
      if (result == -QERR_HIL_WRITE_NOT_SUPPORTED) {
        t_error local_result;
        result = 0;

        /* The hil_write operation is not supported by this card. Write final outputs for each channel type */
        if (num_final_analog_outputs > 0) {
          local_result = hil_write_analog(helicopter_DW.HILInitialize_Card,
            helicopter_P.HILInitialize_analog_output_cha,
            num_final_analog_outputs, &helicopter_DW.HILInitialize_AOVoltages[0]);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (num_final_pwm_outputs > 0) {
          local_result = hil_write_pwm(helicopter_DW.HILInitialize_Card,
            helicopter_P.HILInitialize_pwm_channels, num_final_pwm_outputs,
            &helicopter_DW.HILInitialize_POValues[0]);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (result < 0) {
          msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
            (_rt_error_message));
          rtmSetErrorStatus(helicopter_M, _rt_error_message);
        }
      }
    }

    hil_task_delete_all(helicopter_DW.HILInitialize_Card);
    hil_monitor_delete_all(helicopter_DW.HILInitialize_Card);
    hil_close(helicopter_DW.HILInitialize_Card);
    helicopter_DW.HILInitialize_Card = NULL;
  }

  /* Terminate for ToFile: '<Root>/To File3' */
  {
    FILE *fp = (FILE *) helicopter_DW.ToFile3_PWORK.FilePtr;
    if (fp != (NULL)) {
      char fileName[509] = "p_ref_practical.mat";
      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_M,
                          "Error closing MAT-file p_ref_practical.mat");
        return;
      }

      if ((fp = fopen(fileName, "r+b")) == (NULL)) {
        rtmSetErrorStatus(helicopter_M,
                          "Error reopening MAT-file p_ref_practical.mat");
        return;
      }

      if (rt_WriteMat4FileHeader(fp, 2, helicopter_DW.ToFile3_IWORK.Count,
           "p_ref")) {
        rtmSetErrorStatus(helicopter_M,
                          "Error writing header for p_ref to MAT-file p_ref_practical.mat");
      }

      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_M,
                          "Error closing MAT-file p_ref_practical.mat");
        return;
      }

      helicopter_DW.ToFile3_PWORK.FilePtr = (NULL);
    }
  }

  /* Terminate for ToFile: '<S8>/To File' */
  {
    FILE *fp = (FILE *) helicopter_DW.ToFile_PWORK.FilePtr;
    if (fp != (NULL)) {
      char fileName[509] = "p_dot_practical.mat";
      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_M,
                          "Error closing MAT-file p_dot_practical.mat");
        return;
      }

      if ((fp = fopen(fileName, "r+b")) == (NULL)) {
        rtmSetErrorStatus(helicopter_M,
                          "Error reopening MAT-file p_dot_practical.mat");
        return;
      }

      if (rt_WriteMat4FileHeader(fp, 2, helicopter_DW.ToFile_IWORK.Count,
           "p_dot")) {
        rtmSetErrorStatus(helicopter_M,
                          "Error writing header for p_dot to MAT-file p_dot_practical.mat");
      }

      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_M,
                          "Error closing MAT-file p_dot_practical.mat");
        return;
      }

      helicopter_DW.ToFile_PWORK.FilePtr = (NULL);
    }
  }

  /* Terminate for ToFile: '<S8>/To File1' */
  {
    FILE *fp = (FILE *) helicopter_DW.ToFile1_PWORK.FilePtr;
    if (fp != (NULL)) {
      char fileName[509] = "p_practical.mat";
      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_M, "Error closing MAT-file p_practical.mat");
        return;
      }

      if ((fp = fopen(fileName, "r+b")) == (NULL)) {
        rtmSetErrorStatus(helicopter_M,
                          "Error reopening MAT-file p_practical.mat");
        return;
      }

      if (rt_WriteMat4FileHeader(fp, 2, helicopter_DW.ToFile1_IWORK.Count, "p"))
      {
        rtmSetErrorStatus(helicopter_M,
                          "Error writing header for p to MAT-file p_practical.mat");
      }

      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_M, "Error closing MAT-file p_practical.mat");
        return;
      }

      helicopter_DW.ToFile1_PWORK.FilePtr = (NULL);
    }
  }

  /* Terminate for ToFile: '<S8>/To File2' */
  {
    FILE *fp = (FILE *) helicopter_DW.ToFile2_PWORK.FilePtr;
    if (fp != (NULL)) {
      char fileName[509] = "lambda_practical.mat";
      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_M,
                          "Error closing MAT-file lambda_practical.mat");
        return;
      }

      if ((fp = fopen(fileName, "r+b")) == (NULL)) {
        rtmSetErrorStatus(helicopter_M,
                          "Error reopening MAT-file lambda_practical.mat");
        return;
      }

      if (rt_WriteMat4FileHeader(fp, 2, helicopter_DW.ToFile2_IWORK.Count,
           "lambda")) {
        rtmSetErrorStatus(helicopter_M,
                          "Error writing header for lambda to MAT-file lambda_practical.mat");
      }

      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_M,
                          "Error closing MAT-file lambda_practical.mat");
        return;
      }

      helicopter_DW.ToFile2_PWORK.FilePtr = (NULL);
    }
  }

  /* Terminate for ToFile: '<S8>/To File4' */
  {
    FILE *fp = (FILE *) helicopter_DW.ToFile4_PWORK.FilePtr;
    if (fp != (NULL)) {
      char fileName[509] = "r_practical.mat";
      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_M, "Error closing MAT-file r_practical.mat");
        return;
      }

      if ((fp = fopen(fileName, "r+b")) == (NULL)) {
        rtmSetErrorStatus(helicopter_M,
                          "Error reopening MAT-file r_practical.mat");
        return;
      }

      if (rt_WriteMat4FileHeader(fp, 2, helicopter_DW.ToFile4_IWORK.Count, "r"))
      {
        rtmSetErrorStatus(helicopter_M,
                          "Error writing header for r to MAT-file r_practical.mat");
      }

      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_M, "Error closing MAT-file r_practical.mat");
        return;
      }

      helicopter_DW.ToFile4_PWORK.FilePtr = (NULL);
    }
  }
}

/*========================================================================*
 * Start of Classic call interface                                        *
 *========================================================================*/

/* Solver interface called by GRT_Main */
#ifndef USE_GENERATED_SOLVER

void rt_ODECreateIntegrationData(RTWSolverInfo *si)
{
  UNUSED_PARAMETER(si);
  return;
}                                      /* do nothing */

void rt_ODEDestroyIntegrationData(RTWSolverInfo *si)
{
  UNUSED_PARAMETER(si);
  return;
}                                      /* do nothing */

void rt_ODEUpdateContinuousStates(RTWSolverInfo *si)
{
  UNUSED_PARAMETER(si);
  return;
}                                      /* do nothing */

#endif

void MdlOutputs(int_T tid)
{
  helicopter_output();
  UNUSED_PARAMETER(tid);
}

void MdlUpdate(int_T tid)
{
  helicopter_update();
  UNUSED_PARAMETER(tid);
}

void MdlInitializeSizes(void)
{
}

void MdlInitializeSampleTimes(void)
{
}

void MdlInitialize(void)
{
}

void MdlStart(void)
{
  helicopter_initialize();
}

void MdlTerminate(void)
{
  helicopter_terminate();
}

/* Registration function */
RT_MODEL_helicopter_T *helicopter(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* non-finite (run-time) assignments */
  helicopter_P.Integrator_UpperSat = rtInf;
  helicopter_P.Integrator_LowerSat = rtMinusInf;

  /* initialize real-time model */
  (void) memset((void *)helicopter_M, 0,
                sizeof(RT_MODEL_helicopter_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&helicopter_M->solverInfo,
                          &helicopter_M->Timing.simTimeStep);
    rtsiSetTPtr(&helicopter_M->solverInfo, &rtmGetTPtr(helicopter_M));
    rtsiSetStepSizePtr(&helicopter_M->solverInfo,
                       &helicopter_M->Timing.stepSize0);
    rtsiSetdXPtr(&helicopter_M->solverInfo, &helicopter_M->ModelData.derivs);
    rtsiSetContStatesPtr(&helicopter_M->solverInfo, (real_T **)
                         &helicopter_M->ModelData.contStates);
    rtsiSetNumContStatesPtr(&helicopter_M->solverInfo,
      &helicopter_M->Sizes.numContStates);
    rtsiSetErrorStatusPtr(&helicopter_M->solverInfo, (&rtmGetErrorStatus
      (helicopter_M)));
    rtsiSetRTModelPtr(&helicopter_M->solverInfo, helicopter_M);
  }

  rtsiSetSimTimeStep(&helicopter_M->solverInfo, MAJOR_TIME_STEP);
  helicopter_M->ModelData.intgData.f[0] = helicopter_M->ModelData.odeF[0];
  helicopter_M->ModelData.contStates = ((real_T *) &helicopter_X);
  rtsiSetSolverData(&helicopter_M->solverInfo, (void *)
                    &helicopter_M->ModelData.intgData);
  rtsiSetSolverName(&helicopter_M->solverInfo,"ode1");

  /* Initialize timing info */
  {
    int_T *mdlTsMap = helicopter_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    mdlTsMap[1] = 1;
    helicopter_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    helicopter_M->Timing.sampleTimes = (&helicopter_M->Timing.sampleTimesArray[0]);
    helicopter_M->Timing.offsetTimes = (&helicopter_M->Timing.offsetTimesArray[0]);

    /* task periods */
    helicopter_M->Timing.sampleTimes[0] = (0.0);
    helicopter_M->Timing.sampleTimes[1] = (0.002);

    /* task offsets */
    helicopter_M->Timing.offsetTimes[0] = (0.0);
    helicopter_M->Timing.offsetTimes[1] = (0.0);
  }

  rtmSetTPtr(helicopter_M, &helicopter_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits = helicopter_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    mdlSampleHits[1] = 1;
    helicopter_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(helicopter_M, 35.0);
  helicopter_M->Timing.stepSize0 = 0.002;
  helicopter_M->Timing.stepSize1 = 0.002;

  /* External mode info */
  helicopter_M->Sizes.checksums[0] = (2882654274U);
  helicopter_M->Sizes.checksums[1] = (1864819463U);
  helicopter_M->Sizes.checksums[2] = (1405459987U);
  helicopter_M->Sizes.checksums[3] = (2309035290U);

  {
    static const sysRanDType rtAlwaysEnabled = SUBSYS_RAN_BC_ENABLE;
    static RTWExtModeInfo rt_ExtModeInfo;
    static const sysRanDType *systemRan[1];
    helicopter_M->extModeInfo = (&rt_ExtModeInfo);
    rteiSetSubSystemActiveVectorAddresses(&rt_ExtModeInfo, systemRan);
    systemRan[0] = &rtAlwaysEnabled;
    rteiSetModelMappingInfoPtr(helicopter_M->extModeInfo,
      &helicopter_M->SpecialInfo.mappingInfo);
    rteiSetChecksumsPtr(helicopter_M->extModeInfo, helicopter_M->Sizes.checksums);
    rteiSetTPtr(helicopter_M->extModeInfo, rtmGetTPtr(helicopter_M));
  }

  helicopter_M->solverInfoPtr = (&helicopter_M->solverInfo);
  helicopter_M->Timing.stepSize = (0.002);
  rtsiSetFixedStepSize(&helicopter_M->solverInfo, 0.002);
  rtsiSetSolverMode(&helicopter_M->solverInfo, SOLVER_MODE_SINGLETASKING);

  /* block I/O */
  helicopter_M->ModelData.blockIO = ((void *) &helicopter_B);

  {
    int_T i;
    for (i = 0; i < 6; i++) {
      helicopter_B.Gain1[i] = 0.0;
    }

    helicopter_B.ElevationCounttorad = 0.0;
    helicopter_B.Gain = 0.0;
    helicopter_B.Sum = 0.0;
    helicopter_B.PitchCounttorad = 0.0;
    helicopter_B.Gain_i = 0.0;
    helicopter_B.Gain_g = 0.0;
    helicopter_B.Gain_p = 0.0;
    helicopter_B.Sum3 = 0.0;
    helicopter_B.Gain_d = 0.0;
    helicopter_B.Gain_b = 0.0;
    helicopter_B.Gain_dg = 0.0;
    helicopter_B.Sum5 = 0.0;
    helicopter_B.Sum1 = 0.0;
    helicopter_B.Gain1_f = 0.0;
    helicopter_B.Sum2 = 0.0;
    helicopter_B.K_ei = 0.0;
    helicopter_B.Gain_l = 0.0;
    helicopter_B.BackmotorSaturation = 0.0;
    helicopter_B.FrontmotorSaturation = 0.0;
  }

  /* parameters */
  helicopter_M->ModelData.defaultParam = ((real_T *)&helicopter_P);

  /* states (continuous) */
  {
    real_T *x = (real_T *) &helicopter_X;
    helicopter_M->ModelData.contStates = (x);
    (void) memset((void *)&helicopter_X, 0,
                  sizeof(X_helicopter_T));
  }

  /* states (dwork) */
  helicopter_M->ModelData.dwork = ((void *) &helicopter_DW);
  (void) memset((void *)&helicopter_DW, 0,
                sizeof(DW_helicopter_T));

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      helicopter_DW.HILInitialize_AIMinimums[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      helicopter_DW.HILInitialize_AIMaximums[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      helicopter_DW.HILInitialize_AOMinimums[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      helicopter_DW.HILInitialize_AOMaximums[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      helicopter_DW.HILInitialize_AOVoltages[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      helicopter_DW.HILInitialize_FilterFrequency[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      helicopter_DW.HILInitialize_POSortedFreqs[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      helicopter_DW.HILInitialize_POValues[i] = 0.0;
    }
  }

  helicopter_DW.TimeStampA = 0.0;
  helicopter_DW.LastUAtTimeA = 0.0;
  helicopter_DW.TimeStampB = 0.0;
  helicopter_DW.LastUAtTimeB = 0.0;
  helicopter_DW.HILWriteAnalog_Buffer[0] = 0.0;
  helicopter_DW.HILWriteAnalog_Buffer[1] = 0.0;

  /* data type transition information */
  {
    static DataTypeTransInfo dtInfo;
    (void) memset((char_T *) &dtInfo, 0,
                  sizeof(dtInfo));
    helicopter_M->SpecialInfo.mappingInfo = (&dtInfo);
    dtInfo.numDataTypes = 16;
    dtInfo.dataTypeSizes = &rtDataTypeSizes[0];
    dtInfo.dataTypeNames = &rtDataTypeNames[0];

    /* Block I/O transition table */
    dtInfo.B = &rtBTransTable;

    /* Parameters transition table */
    dtInfo.P = &rtPTransTable;
  }

  /* Initialize Sizes */
  helicopter_M->Sizes.numContStates = (4);/* Number of continuous states */
  helicopter_M->Sizes.numY = (0);      /* Number of model outputs */
  helicopter_M->Sizes.numU = (0);      /* Number of model inputs */
  helicopter_M->Sizes.sysDirFeedThru = (0);/* The model is not direct feedthrough */
  helicopter_M->Sizes.numSampTimes = (2);/* Number of sample times */
  helicopter_M->Sizes.numBlocks = (71);/* Number of blocks */
  helicopter_M->Sizes.numBlockIO = (20);/* Number of block outputs */
  helicopter_M->Sizes.numBlockPrms = (148);/* Sum of parameter "widths" */
  return helicopter_M;
}

/*========================================================================*
 * End of Classic call interface                                          *
 *========================================================================*/
