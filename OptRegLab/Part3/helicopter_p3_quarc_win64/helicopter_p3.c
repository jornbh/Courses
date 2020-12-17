/*
 * helicopter_p3.c
 *
 * Code generation for model "helicopter_p3".
 *
 * Model version              : 1.206
 * Simulink Coder version : 8.6 (R2014a) 27-Dec-2013
 * C source code generated on : Wed Feb 28 10:48:18 2018
 *
 * Target selection: quarc_win64.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: 32-bit Generic
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */
#include "helicopter_p3.h"
#include "helicopter_p3_private.h"
#include "helicopter_p3_dt.h"

/* Block signals (auto storage) */
B_helicopter_p3_T helicopter_p3_B;

/* Continuous states */
X_helicopter_p3_T helicopter_p3_X;

/* Block states (auto storage) */
DW_helicopter_p3_T helicopter_p3_DW;

/* Real-time model */
RT_MODEL_helicopter_p3_T helicopter_p3_M_;
RT_MODEL_helicopter_p3_T *const helicopter_p3_M = &helicopter_p3_M_;

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
  helicopter_p3_derivatives();
  rtsiSetT(si, tnew);
  for (i = 0; i < nXc; i++) {
    *x += h * f0[i];
    x++;
  }

  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

/* Model output function */
void helicopter_p3_output(void)
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
  if (rtmIsMajorTimeStep(helicopter_p3_M)) {
    /* set solver stop time */
    if (!(helicopter_p3_M->Timing.clockTick0+1)) {
      rtsiSetSolverStopTime(&helicopter_p3_M->solverInfo,
                            ((helicopter_p3_M->Timing.clockTickH0 + 1) *
        helicopter_p3_M->Timing.stepSize0 * 4294967296.0));
    } else {
      rtsiSetSolverStopTime(&helicopter_p3_M->solverInfo,
                            ((helicopter_p3_M->Timing.clockTick0 + 1) *
        helicopter_p3_M->Timing.stepSize0 + helicopter_p3_M->Timing.clockTickH0 *
        helicopter_p3_M->Timing.stepSize0 * 4294967296.0));
    }
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(helicopter_p3_M)) {
    helicopter_p3_M->Timing.t[0] = rtsiGetT(&helicopter_p3_M->solverInfo);
  }

  if (rtmIsMajorTimeStep(helicopter_p3_M)) {
    /* S-Function (hil_read_encoder_timebase_block): '<S5>/HIL Read Encoder Timebase' */

    /* S-Function Block: helicopter_p3/Helicopter_interface/HIL Read Encoder Timebase (hil_read_encoder_timebase_block) */
    {
      t_error result;
      result = hil_task_read_encoder
        (helicopter_p3_DW.HILReadEncoderTimebase_Task, 1,
         &helicopter_p3_DW.HILReadEncoderTimebase_Buffer[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p3_M, _rt_error_message);
      } else {
        rtb_HILReadEncoderTimebase_o1 =
          helicopter_p3_DW.HILReadEncoderTimebase_Buffer[0];
        rtb_HILReadEncoderTimebase_o2 =
          helicopter_p3_DW.HILReadEncoderTimebase_Buffer[1];
        rtb_HILReadEncoderTimebase_o3 =
          helicopter_p3_DW.HILReadEncoderTimebase_Buffer[2];
      }
    }

    /* Gain: '<S5>/Elevation: Count to rad' */
    helicopter_p3_B.ElevationCounttorad =
      helicopter_p3_P.ElevationCounttorad_Gain * rtb_HILReadEncoderTimebase_o3;

    /* Gain: '<S9>/Gain' */
    helicopter_p3_B.Gain = helicopter_p3_P.Gain_Gain *
      helicopter_p3_B.ElevationCounttorad;

    /* Sum: '<Root>/Sum' incorporates:
     *  Constant: '<Root>/elavation_offset [deg]'
     */
    helicopter_p3_B.Sum = helicopter_p3_B.Gain +
      helicopter_p3_P.elavation_offsetdeg_Value;

    /* Gain: '<S5>/Pitch: Count to rad' */
    helicopter_p3_B.PitchCounttorad = helicopter_p3_P.PitchCounttorad_Gain *
      rtb_HILReadEncoderTimebase_o2;

    /* Gain: '<S11>/Gain' */
    helicopter_p3_B.Gain_i = helicopter_p3_P.Gain_Gain_a *
      helicopter_p3_B.PitchCounttorad;
  }

  /* FromWorkspace: '<Root>/     ' */
  {
    real_T *pDataValues = (real_T *) helicopter_p3_DW._PWORK.DataPtr;
    real_T *pTimeValues = (real_T *) helicopter_p3_DW._PWORK.TimePtr;
    int_T currTimeIndex = helicopter_p3_DW._IWORK.PrevIndex;
    real_T t = helicopter_p3_M->Timing.t[0];

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

    helicopter_p3_DW._IWORK.PrevIndex = currTimeIndex;

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
    real_T *pDataValues = (real_T *) helicopter_p3_DW._PWORK_k.DataPtr;
    real_T *pTimeValues = (real_T *) helicopter_p3_DW._PWORK_k.TimePtr;
    int_T currTimeIndex = helicopter_p3_DW._IWORK_g.PrevIndex;
    real_T t = helicopter_p3_M->Timing.t[0];

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

    helicopter_p3_DW._IWORK_g.PrevIndex = currTimeIndex;

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

  if (rtmIsMajorTimeStep(helicopter_p3_M)) {
    /* Gain: '<S5>/Gain' incorporates:
     *  Gain: '<S5>/Travel: Count to rad'
     */
    helicopter_p3_B.Gain_g = helicopter_p3_P.TravelCounttorad_Gain *
      rtb_HILReadEncoderTimebase_o1 * helicopter_p3_P.Gain_Gain_b;

    /* Gain: '<S14>/Gain' */
    helicopter_p3_B.Gain_p = helicopter_p3_P.Gain_Gain_ar *
      helicopter_p3_B.Gain_g;

    /* Sum: '<Root>/Sum3' incorporates:
     *  Constant: '<Root>/Travel offset [deg]'
     */
    helicopter_p3_B.Sum3 = helicopter_p3_B.Gain_p +
      helicopter_p3_P.Traveloffsetdeg_Value;
  }

  /* TransferFcn: '<S5>/Travel: Transfer Fcn' */
  rtb_Backgain = 0.0;
  rtb_Backgain += helicopter_p3_P.TravelTransferFcn_C *
    helicopter_p3_X.TravelTransferFcn_CSTATE;
  rtb_Backgain += helicopter_p3_P.TravelTransferFcn_D * helicopter_p3_B.Gain_g;

  /* Gain: '<S15>/Gain' */
  helicopter_p3_B.Gain_d = helicopter_p3_P.Gain_Gain_l * rtb_Backgain;

  /* TransferFcn: '<S5>/Pitch: Transfer Fcn' */
  rtb_Backgain = 0.0;
  rtb_Backgain += helicopter_p3_P.PitchTransferFcn_C *
    helicopter_p3_X.PitchTransferFcn_CSTATE;
  rtb_Backgain += helicopter_p3_P.PitchTransferFcn_D *
    helicopter_p3_B.PitchCounttorad;

  /* Gain: '<S12>/Gain' */
  helicopter_p3_B.Gain_b = helicopter_p3_P.Gain_Gain_ae * rtb_Backgain;

  /* TransferFcn: '<S5>/Elevation: Transfer Fcn' */
  rtb_Backgain = 0.0;
  rtb_Backgain += helicopter_p3_P.ElevationTransferFcn_C *
    helicopter_p3_X.ElevationTransferFcn_CSTATE;
  rtb_Backgain += helicopter_p3_P.ElevationTransferFcn_D *
    helicopter_p3_B.ElevationCounttorad;

  /* Gain: '<S10>/Gain' */
  helicopter_p3_B.Gain_dg = helicopter_p3_P.Gain_Gain_n * rtb_Backgain;

  /* Gain: '<S3>/Gain1' */
  helicopter_p3_B.Gain1[0] = helicopter_p3_P.Gain1_Gain * helicopter_p3_B.Sum3;
  helicopter_p3_B.Gain1[1] = helicopter_p3_P.Gain1_Gain * helicopter_p3_B.Gain_d;
  helicopter_p3_B.Gain1[2] = helicopter_p3_P.Gain1_Gain * helicopter_p3_B.Gain_i;
  helicopter_p3_B.Gain1[3] = helicopter_p3_P.Gain1_Gain * helicopter_p3_B.Gain_b;
  helicopter_p3_B.Gain1[4] = helicopter_p3_P.Gain1_Gain * helicopter_p3_B.Sum;
  helicopter_p3_B.Gain1[5] = helicopter_p3_P.Gain1_Gain *
    helicopter_p3_B.Gain_dg;

  /* Sum: '<S6>/Sum4' */
  rtb_Sum4[0] = helicopter_p3_B.Gain1[0] - rtb_Sum4[0];
  rtb_Sum4[1] = helicopter_p3_B.Gain1[1] - rtb_Sum4[1];
  rtb_Sum4[2] = helicopter_p3_B.Gain1[2] - rtb_Sum4[2];
  rtb_Sum4[3] = helicopter_p3_B.Gain1[3] - rtb_Sum4[3];

  /* Gain: '<S6>/Gain' */
  rtb_Backgain = ((helicopter_p3_P.K[0] * rtb_Sum4[0] + helicopter_p3_P.K[1] *
                   rtb_Sum4[1]) + helicopter_p3_P.K[2] * rtb_Sum4[2]) +
    helicopter_p3_P.K[3] * rtb_Sum4[3];

  /* Sum: '<S6>/Sum5' */
  helicopter_p3_B.Sum5 = rtb_Derivative - rtb_Backgain;
  if (rtmIsMajorTimeStep(helicopter_p3_M)) {
    /* ToFile: '<Root>/To File3' */
    {
      if (!(++helicopter_p3_DW.ToFile3_IWORK.Decimation % 1) &&
          (helicopter_p3_DW.ToFile3_IWORK.Count*2)+1 < 100000000 ) {
        FILE *fp = (FILE *) helicopter_p3_DW.ToFile3_PWORK.FilePtr;
        if (fp != (NULL)) {
          real_T u[2];
          helicopter_p3_DW.ToFile3_IWORK.Decimation = 0;
          u[0] = helicopter_p3_M->Timing.t[1];
          u[1] = helicopter_p3_B.Sum5;
          if (fwrite(u, sizeof(real_T), 2, fp) != 2) {
            rtmSetErrorStatus(helicopter_p3_M,
                              "Error writing to MAT-file p_ref_practical.mat");
            return;
          }

          if (((++helicopter_p3_DW.ToFile3_IWORK.Count)*2)+1 >= 100000000) {
            (void)fprintf(stdout,
                          "*** The ToFile block will stop logging data before\n"
                          "    the simulation has ended, because it has reached\n"
                          "    the maximum number of elements (100000000)\n"
                          "    allowed in MAT-file p_ref_practical.mat.\n");
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
  helicopter_p3_B.Sum1 = ((helicopter_p3_B.Sum5 - helicopter_p3_B.Gain1[2]) *
    helicopter_p3_P.K_pp - helicopter_p3_P.K_pd * helicopter_p3_B.Gain1[3]) +
    helicopter_p3_P.Vd_ff;
  if (rtmIsMajorTimeStep(helicopter_p3_M)) {
  }

  /* Integrator: '<S4>/Integrator'
   *
   * Regarding '<S4>/Integrator':
   *  Limited Integrator
   */
  if (helicopter_p3_X.Integrator_CSTATE >= helicopter_p3_P.Integrator_UpperSat )
  {
    helicopter_p3_X.Integrator_CSTATE = helicopter_p3_P.Integrator_UpperSat;
  } else if (helicopter_p3_X.Integrator_CSTATE <=
             (helicopter_p3_P.Integrator_LowerSat) ) {
    helicopter_p3_X.Integrator_CSTATE = (helicopter_p3_P.Integrator_LowerSat);
  }

  rtb_Backgain = helicopter_p3_X.Integrator_CSTATE;
  if (rtmIsMajorTimeStep(helicopter_p3_M)) {
    /* Gain: '<S2>/Gain1' incorporates:
     *  Constant: '<Root>/elevation_ref'
     */
    helicopter_p3_B.Gain1_f = helicopter_p3_P.Gain1_Gain_m *
      helicopter_p3_P.elevation_ref_Value;
  }

  /* Sum: '<S4>/Sum' */
  rtb_Derivative = helicopter_p3_B.Gain1_f - helicopter_p3_B.Gain1[4];

  /* Sum: '<Root>/Sum2' incorporates:
   *  Constant: '<Root>/Vs_bias'
   *  Gain: '<S4>/K_ed'
   *  Gain: '<S4>/K_ep'
   *  Sum: '<S4>/Sum1'
   */
  helicopter_p3_B.Sum2 = ((helicopter_p3_P.K_ep * rtb_Derivative + rtb_Backgain)
    - helicopter_p3_P.K_ed * helicopter_p3_B.Gain1[5]) + helicopter_p3_P.Vs_ff;
  if (rtmIsMajorTimeStep(helicopter_p3_M)) {
    /* ToFile: '<S8>/To File' */
    {
      if (!(++helicopter_p3_DW.ToFile_IWORK.Decimation % 1) &&
          (helicopter_p3_DW.ToFile_IWORK.Count*2)+1 < 100000000 ) {
        FILE *fp = (FILE *) helicopter_p3_DW.ToFile_PWORK.FilePtr;
        if (fp != (NULL)) {
          real_T u[2];
          helicopter_p3_DW.ToFile_IWORK.Decimation = 0;
          u[0] = helicopter_p3_M->Timing.t[1];
          u[1] = helicopter_p3_B.Gain1[3];
          if (fwrite(u, sizeof(real_T), 2, fp) != 2) {
            rtmSetErrorStatus(helicopter_p3_M,
                              "Error writing to MAT-file p_dot_practical.mat");
            return;
          }

          if (((++helicopter_p3_DW.ToFile_IWORK.Count)*2)+1 >= 100000000) {
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
      if (!(++helicopter_p3_DW.ToFile1_IWORK.Decimation % 1) &&
          (helicopter_p3_DW.ToFile1_IWORK.Count*2)+1 < 100000000 ) {
        FILE *fp = (FILE *) helicopter_p3_DW.ToFile1_PWORK.FilePtr;
        if (fp != (NULL)) {
          real_T u[2];
          helicopter_p3_DW.ToFile1_IWORK.Decimation = 0;
          u[0] = helicopter_p3_M->Timing.t[1];
          u[1] = helicopter_p3_B.Gain1[2];
          if (fwrite(u, sizeof(real_T), 2, fp) != 2) {
            rtmSetErrorStatus(helicopter_p3_M,
                              "Error writing to MAT-file p_practical.mat");
            return;
          }

          if (((++helicopter_p3_DW.ToFile1_IWORK.Count)*2)+1 >= 100000000) {
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
      if (!(++helicopter_p3_DW.ToFile2_IWORK.Decimation % 1) &&
          (helicopter_p3_DW.ToFile2_IWORK.Count*2)+1 < 100000000 ) {
        FILE *fp = (FILE *) helicopter_p3_DW.ToFile2_PWORK.FilePtr;
        if (fp != (NULL)) {
          real_T u[2];
          helicopter_p3_DW.ToFile2_IWORK.Decimation = 0;
          u[0] = helicopter_p3_M->Timing.t[1];
          u[1] = helicopter_p3_B.Gain1[0];
          if (fwrite(u, sizeof(real_T), 2, fp) != 2) {
            rtmSetErrorStatus(helicopter_p3_M,
                              "Error writing to MAT-file lambda_practical.mat");
            return;
          }

          if (((++helicopter_p3_DW.ToFile2_IWORK.Count)*2)+1 >= 100000000) {
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
      if (!(++helicopter_p3_DW.ToFile4_IWORK.Decimation % 1) &&
          (helicopter_p3_DW.ToFile4_IWORK.Count*2)+1 < 100000000 ) {
        FILE *fp = (FILE *) helicopter_p3_DW.ToFile4_PWORK.FilePtr;
        if (fp != (NULL)) {
          real_T u[2];
          helicopter_p3_DW.ToFile4_IWORK.Decimation = 0;
          u[0] = helicopter_p3_M->Timing.t[1];
          u[1] = helicopter_p3_B.Gain1[1];
          if (fwrite(u, sizeof(real_T), 2, fp) != 2) {
            rtmSetErrorStatus(helicopter_p3_M,
                              "Error writing to MAT-file r_practical.mat");
            return;
          }

          if (((++helicopter_p3_DW.ToFile4_IWORK.Count)*2)+1 >= 100000000) {
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

  /* Gain: '<S1>/Back gain' incorporates:
   *  Sum: '<S1>/Subtract'
   */
  rtb_Backgain = (helicopter_p3_B.Sum2 - helicopter_p3_B.Sum1) *
    helicopter_p3_P.Backgain_Gain;

  /* Gain: '<S4>/K_ei' */
  helicopter_p3_B.K_ei = helicopter_p3_P.K_ei * rtb_Derivative;
  if (rtmIsMajorTimeStep(helicopter_p3_M)) {
  }

  /* Derivative: '<S5>/Derivative' */
  if ((helicopter_p3_DW.TimeStampA >= helicopter_p3_M->Timing.t[0]) &&
      (helicopter_p3_DW.TimeStampB >= helicopter_p3_M->Timing.t[0])) {
    rtb_Derivative = 0.0;
  } else {
    lastTime = helicopter_p3_DW.TimeStampA;
    lastU = &helicopter_p3_DW.LastUAtTimeA;
    if (helicopter_p3_DW.TimeStampA < helicopter_p3_DW.TimeStampB) {
      if (helicopter_p3_DW.TimeStampB < helicopter_p3_M->Timing.t[0]) {
        lastTime = helicopter_p3_DW.TimeStampB;
        lastU = &helicopter_p3_DW.LastUAtTimeB;
      }
    } else {
      if (helicopter_p3_DW.TimeStampA >= helicopter_p3_M->Timing.t[0]) {
        lastTime = helicopter_p3_DW.TimeStampB;
        lastU = &helicopter_p3_DW.LastUAtTimeB;
      }
    }

    rtb_Derivative = (helicopter_p3_B.PitchCounttorad - *lastU) /
      (helicopter_p3_M->Timing.t[0] - lastTime);
  }

  /* End of Derivative: '<S5>/Derivative' */

  /* Gain: '<S13>/Gain' */
  helicopter_p3_B.Gain_l = helicopter_p3_P.Gain_Gain_a1 * rtb_Derivative;
  if (rtmIsMajorTimeStep(helicopter_p3_M)) {
  }

  /* Saturate: '<S5>/Back motor: Saturation' */
  if (rtb_Backgain > helicopter_p3_P.BackmotorSaturation_UpperSat) {
    helicopter_p3_B.BackmotorSaturation =
      helicopter_p3_P.BackmotorSaturation_UpperSat;
  } else if (rtb_Backgain < helicopter_p3_P.BackmotorSaturation_LowerSat) {
    helicopter_p3_B.BackmotorSaturation =
      helicopter_p3_P.BackmotorSaturation_LowerSat;
  } else {
    helicopter_p3_B.BackmotorSaturation = rtb_Backgain;
  }

  /* End of Saturate: '<S5>/Back motor: Saturation' */
  if (rtmIsMajorTimeStep(helicopter_p3_M)) {
  }

  /* Gain: '<S1>/Front gain' incorporates:
   *  Sum: '<S1>/Add'
   */
  lastTime = (helicopter_p3_B.Sum1 + helicopter_p3_B.Sum2) *
    helicopter_p3_P.Frontgain_Gain;

  /* Saturate: '<S5>/Front motor: Saturation' */
  if (lastTime > helicopter_p3_P.FrontmotorSaturation_UpperSat) {
    helicopter_p3_B.FrontmotorSaturation =
      helicopter_p3_P.FrontmotorSaturation_UpperSat;
  } else if (lastTime < helicopter_p3_P.FrontmotorSaturation_LowerSat) {
    helicopter_p3_B.FrontmotorSaturation =
      helicopter_p3_P.FrontmotorSaturation_LowerSat;
  } else {
    helicopter_p3_B.FrontmotorSaturation = lastTime;
  }

  /* End of Saturate: '<S5>/Front motor: Saturation' */
  if (rtmIsMajorTimeStep(helicopter_p3_M)) {
    /* S-Function (hil_write_analog_block): '<S5>/HIL Write Analog' */

    /* S-Function Block: helicopter_p3/Helicopter_interface/HIL Write Analog (hil_write_analog_block) */
    {
      t_error result;
      helicopter_p3_DW.HILWriteAnalog_Buffer[0] =
        helicopter_p3_B.FrontmotorSaturation;
      helicopter_p3_DW.HILWriteAnalog_Buffer[1] =
        helicopter_p3_B.BackmotorSaturation;
      result = hil_write_analog(helicopter_p3_DW.HILInitialize_Card,
        helicopter_p3_P.HILWriteAnalog_channels, 2,
        &helicopter_p3_DW.HILWriteAnalog_Buffer[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p3_M, _rt_error_message);
      }
    }
  }
}

/* Model update function */
void helicopter_p3_update(void)
{
  real_T *lastU;

  /* Update for Derivative: '<S5>/Derivative' */
  if (helicopter_p3_DW.TimeStampA == (rtInf)) {
    helicopter_p3_DW.TimeStampA = helicopter_p3_M->Timing.t[0];
    lastU = &helicopter_p3_DW.LastUAtTimeA;
  } else if (helicopter_p3_DW.TimeStampB == (rtInf)) {
    helicopter_p3_DW.TimeStampB = helicopter_p3_M->Timing.t[0];
    lastU = &helicopter_p3_DW.LastUAtTimeB;
  } else if (helicopter_p3_DW.TimeStampA < helicopter_p3_DW.TimeStampB) {
    helicopter_p3_DW.TimeStampA = helicopter_p3_M->Timing.t[0];
    lastU = &helicopter_p3_DW.LastUAtTimeA;
  } else {
    helicopter_p3_DW.TimeStampB = helicopter_p3_M->Timing.t[0];
    lastU = &helicopter_p3_DW.LastUAtTimeB;
  }

  *lastU = helicopter_p3_B.PitchCounttorad;

  /* End of Update for Derivative: '<S5>/Derivative' */
  if (rtmIsMajorTimeStep(helicopter_p3_M)) {
    rt_ertODEUpdateContinuousStates(&helicopter_p3_M->solverInfo);
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
  if (!(++helicopter_p3_M->Timing.clockTick0)) {
    ++helicopter_p3_M->Timing.clockTickH0;
  }

  helicopter_p3_M->Timing.t[0] = rtsiGetSolverStopTime
    (&helicopter_p3_M->solverInfo);

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
    if (!(++helicopter_p3_M->Timing.clockTick1)) {
      ++helicopter_p3_M->Timing.clockTickH1;
    }

    helicopter_p3_M->Timing.t[1] = helicopter_p3_M->Timing.clockTick1 *
      helicopter_p3_M->Timing.stepSize1 + helicopter_p3_M->Timing.clockTickH1 *
      helicopter_p3_M->Timing.stepSize1 * 4294967296.0;
  }
}

/* Derivatives for root system: '<Root>' */
void helicopter_p3_derivatives(void)
{
  XDot_helicopter_p3_T *_rtXdot;
  _rtXdot = ((XDot_helicopter_p3_T *) helicopter_p3_M->ModelData.derivs);

  /* Derivatives for TransferFcn: '<S5>/Travel: Transfer Fcn' */
  _rtXdot->TravelTransferFcn_CSTATE = 0.0;
  _rtXdot->TravelTransferFcn_CSTATE += helicopter_p3_P.TravelTransferFcn_A *
    helicopter_p3_X.TravelTransferFcn_CSTATE;
  _rtXdot->TravelTransferFcn_CSTATE += helicopter_p3_B.Gain_g;

  /* Derivatives for TransferFcn: '<S5>/Pitch: Transfer Fcn' */
  _rtXdot->PitchTransferFcn_CSTATE = 0.0;
  _rtXdot->PitchTransferFcn_CSTATE += helicopter_p3_P.PitchTransferFcn_A *
    helicopter_p3_X.PitchTransferFcn_CSTATE;
  _rtXdot->PitchTransferFcn_CSTATE += helicopter_p3_B.PitchCounttorad;

  /* Derivatives for TransferFcn: '<S5>/Elevation: Transfer Fcn' */
  _rtXdot->ElevationTransferFcn_CSTATE = 0.0;
  _rtXdot->ElevationTransferFcn_CSTATE += helicopter_p3_P.ElevationTransferFcn_A
    * helicopter_p3_X.ElevationTransferFcn_CSTATE;
  _rtXdot->ElevationTransferFcn_CSTATE += helicopter_p3_B.ElevationCounttorad;

  /* Derivatives for Integrator: '<S4>/Integrator' */
  {
    boolean_T lsat;
    boolean_T usat;
    lsat = ( helicopter_p3_X.Integrator_CSTATE <=
            (helicopter_p3_P.Integrator_LowerSat) );
    usat = ( helicopter_p3_X.Integrator_CSTATE >=
            helicopter_p3_P.Integrator_UpperSat );
    if ((!lsat && !usat) ||
        (lsat && (helicopter_p3_B.K_ei > 0)) ||
        (usat && (helicopter_p3_B.K_ei < 0)) ) {
      ((XDot_helicopter_p3_T *) helicopter_p3_M->ModelData.derivs)
        ->Integrator_CSTATE = helicopter_p3_B.K_ei;
    } else {
      /* in saturation */
      ((XDot_helicopter_p3_T *) helicopter_p3_M->ModelData.derivs)
        ->Integrator_CSTATE = 0.0;
    }
  }
}

/* Model initialize function */
void helicopter_p3_initialize(void)
{
  /* Start for S-Function (hil_initialize_block): '<Root>/HIL Initialize' */

  /* S-Function Block: helicopter_p3/HIL Initialize (hil_initialize_block) */
  {
    t_int result;
    t_boolean is_switching;
    result = hil_open("q8_usb", "0", &helicopter_p3_DW.HILInitialize_Card);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(helicopter_p3_M, _rt_error_message);
      return;
    }

    is_switching = false;
    result = hil_set_card_specific_options(helicopter_p3_DW.HILInitialize_Card,
      "update_rate=normal;decimation=1", 32);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(helicopter_p3_M, _rt_error_message);
      return;
    }

    result = hil_watchdog_clear(helicopter_p3_DW.HILInitialize_Card);
    if (result < 0 && result != -QERR_HIL_WATCHDOG_CLEAR) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(helicopter_p3_M, _rt_error_message);
      return;
    }

    if ((helicopter_p3_P.HILInitialize_set_analog_input_ && !is_switching) ||
        (helicopter_p3_P.HILInitialize_set_analog_inpu_m && is_switching)) {
      {
        int_T i1;
        real_T *dw_AIMinimums = &helicopter_p3_DW.HILInitialize_AIMinimums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AIMinimums[i1] = helicopter_p3_P.HILInitialize_analog_input_mini;
        }
      }

      {
        int_T i1;
        real_T *dw_AIMaximums = &helicopter_p3_DW.HILInitialize_AIMaximums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AIMaximums[i1] = helicopter_p3_P.HILInitialize_analog_input_maxi;
        }
      }

      result = hil_set_analog_input_ranges(helicopter_p3_DW.HILInitialize_Card,
        helicopter_p3_P.HILInitialize_analog_input_chan, 8U,
        &helicopter_p3_DW.HILInitialize_AIMinimums[0],
        &helicopter_p3_DW.HILInitialize_AIMaximums[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p3_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_p3_P.HILInitialize_set_analog_output && !is_switching) ||
        (helicopter_p3_P.HILInitialize_set_analog_outp_b && is_switching)) {
      {
        int_T i1;
        real_T *dw_AOMinimums = &helicopter_p3_DW.HILInitialize_AOMinimums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOMinimums[i1] = helicopter_p3_P.HILInitialize_analog_output_min;
        }
      }

      {
        int_T i1;
        real_T *dw_AOMaximums = &helicopter_p3_DW.HILInitialize_AOMaximums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOMaximums[i1] = helicopter_p3_P.HILInitialize_analog_output_max;
        }
      }

      result = hil_set_analog_output_ranges(helicopter_p3_DW.HILInitialize_Card,
        helicopter_p3_P.HILInitialize_analog_output_cha, 8U,
        &helicopter_p3_DW.HILInitialize_AOMinimums[0],
        &helicopter_p3_DW.HILInitialize_AOMaximums[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p3_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_p3_P.HILInitialize_set_analog_outp_e && !is_switching) ||
        (helicopter_p3_P.HILInitialize_set_analog_outp_j && is_switching)) {
      {
        int_T i1;
        real_T *dw_AOVoltages = &helicopter_p3_DW.HILInitialize_AOVoltages[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOVoltages[i1] = helicopter_p3_P.HILInitialize_initial_analog_ou;
        }
      }

      result = hil_write_analog(helicopter_p3_DW.HILInitialize_Card,
        helicopter_p3_P.HILInitialize_analog_output_cha, 8U,
        &helicopter_p3_DW.HILInitialize_AOVoltages[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p3_M, _rt_error_message);
        return;
      }
    }

    if (helicopter_p3_P.HILInitialize_set_analog_outp_p) {
      {
        int_T i1;
        real_T *dw_AOVoltages = &helicopter_p3_DW.HILInitialize_AOVoltages[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOVoltages[i1] = helicopter_p3_P.HILInitialize_watchdog_analog_o;
        }
      }

      result = hil_watchdog_set_analog_expiration_state
        (helicopter_p3_DW.HILInitialize_Card,
         helicopter_p3_P.HILInitialize_analog_output_cha, 8U,
         &helicopter_p3_DW.HILInitialize_AOVoltages[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p3_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_p3_P.HILInitialize_set_encoder_param && !is_switching) ||
        (helicopter_p3_P.HILInitialize_set_encoder_par_m && is_switching)) {
      {
        int_T i1;
        int32_T *dw_QuadratureModes =
          &helicopter_p3_DW.HILInitialize_QuadratureModes[0];
        for (i1=0; i1 < 8; i1++) {
          dw_QuadratureModes[i1] = helicopter_p3_P.HILInitialize_quadrature;
        }
      }

      result = hil_set_encoder_quadrature_mode
        (helicopter_p3_DW.HILInitialize_Card,
         helicopter_p3_P.HILInitialize_encoder_channels, 8U,
         (t_encoder_quadrature_mode *)
         &helicopter_p3_DW.HILInitialize_QuadratureModes[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p3_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_p3_P.HILInitialize_set_encoder_count && !is_switching) ||
        (helicopter_p3_P.HILInitialize_set_encoder_cou_k && is_switching)) {
      {
        int_T i1;
        int32_T *dw_InitialEICounts =
          &helicopter_p3_DW.HILInitialize_InitialEICounts[0];
        for (i1=0; i1 < 8; i1++) {
          dw_InitialEICounts[i1] =
            helicopter_p3_P.HILInitialize_initial_encoder_c;
        }
      }

      result = hil_set_encoder_counts(helicopter_p3_DW.HILInitialize_Card,
        helicopter_p3_P.HILInitialize_encoder_channels, 8U,
        &helicopter_p3_DW.HILInitialize_InitialEICounts[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p3_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_p3_P.HILInitialize_set_pwm_params_at && !is_switching) ||
        (helicopter_p3_P.HILInitialize_set_pwm_params__f && is_switching)) {
      uint32_T num_duty_cycle_modes = 0;
      uint32_T num_frequency_modes = 0;

      {
        int_T i1;
        int32_T *dw_POModeValues = &helicopter_p3_DW.HILInitialize_POModeValues
          [0];
        for (i1=0; i1 < 8; i1++) {
          dw_POModeValues[i1] = helicopter_p3_P.HILInitialize_pwm_modes;
        }
      }

      result = hil_set_pwm_mode(helicopter_p3_DW.HILInitialize_Card,
        helicopter_p3_P.HILInitialize_pwm_channels, 8U, (t_pwm_mode *)
        &helicopter_p3_DW.HILInitialize_POModeValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p3_M, _rt_error_message);
        return;
      }

      {
        int_T i1;
        const uint32_T *p_HILInitialize_pwm_channels =
          helicopter_p3_P.HILInitialize_pwm_channels;
        int32_T *dw_POModeValues = &helicopter_p3_DW.HILInitialize_POModeValues
          [0];
        for (i1=0; i1 < 8; i1++) {
          if (dw_POModeValues[i1] == PWM_DUTY_CYCLE_MODE || dw_POModeValues[i1] ==
              PWM_ONE_SHOT_MODE || dw_POModeValues[i1] == PWM_TIME_MODE) {
            helicopter_p3_DW.HILInitialize_POSortedChans[num_duty_cycle_modes] =
              p_HILInitialize_pwm_channels[i1];
            helicopter_p3_DW.HILInitialize_POSortedFreqs[num_duty_cycle_modes] =
              helicopter_p3_P.HILInitialize_pwm_frequency;
            num_duty_cycle_modes++;
          } else {
            helicopter_p3_DW.HILInitialize_POSortedChans[7U -
              num_frequency_modes] = p_HILInitialize_pwm_channels[i1];
            helicopter_p3_DW.HILInitialize_POSortedFreqs[7U -
              num_frequency_modes] = helicopter_p3_P.HILInitialize_pwm_frequency;
            num_frequency_modes++;
          }
        }
      }

      if (num_duty_cycle_modes > 0) {
        result = hil_set_pwm_frequency(helicopter_p3_DW.HILInitialize_Card,
          &helicopter_p3_DW.HILInitialize_POSortedChans[0], num_duty_cycle_modes,
          &helicopter_p3_DW.HILInitialize_POSortedFreqs[0]);
        if (result < 0) {
          msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
            (_rt_error_message));
          rtmSetErrorStatus(helicopter_p3_M, _rt_error_message);
          return;
        }
      }

      if (num_frequency_modes > 0) {
        result = hil_set_pwm_duty_cycle(helicopter_p3_DW.HILInitialize_Card,
          &helicopter_p3_DW.HILInitialize_POSortedChans[num_duty_cycle_modes],
          num_frequency_modes,
          &helicopter_p3_DW.HILInitialize_POSortedFreqs[num_duty_cycle_modes]);
        if (result < 0) {
          msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
            (_rt_error_message));
          rtmSetErrorStatus(helicopter_p3_M, _rt_error_message);
          return;
        }
      }

      {
        int_T i1;
        int32_T *dw_POModeValues = &helicopter_p3_DW.HILInitialize_POModeValues
          [0];
        for (i1=0; i1 < 8; i1++) {
          dw_POModeValues[i1] = helicopter_p3_P.HILInitialize_pwm_configuration;
        }
      }

      {
        int_T i1;
        int32_T *dw_POAlignValues =
          &helicopter_p3_DW.HILInitialize_POAlignValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POAlignValues[i1] = helicopter_p3_P.HILInitialize_pwm_alignment;
        }
      }

      {
        int_T i1;
        int32_T *dw_POPolarityVals =
          &helicopter_p3_DW.HILInitialize_POPolarityVals[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POPolarityVals[i1] = helicopter_p3_P.HILInitialize_pwm_polarity;
        }
      }

      result = hil_set_pwm_configuration(helicopter_p3_DW.HILInitialize_Card,
        helicopter_p3_P.HILInitialize_pwm_channels, 8U,
        (t_pwm_configuration *) &helicopter_p3_DW.HILInitialize_POModeValues[0],
        (t_pwm_alignment *) &helicopter_p3_DW.HILInitialize_POAlignValues[0],
        (t_pwm_polarity *) &helicopter_p3_DW.HILInitialize_POPolarityVals[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p3_M, _rt_error_message);
        return;
      }

      {
        int_T i1;
        real_T *dw_POSortedFreqs =
          &helicopter_p3_DW.HILInitialize_POSortedFreqs[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POSortedFreqs[i1] = helicopter_p3_P.HILInitialize_pwm_leading_deadb;
        }
      }

      {
        int_T i1;
        real_T *dw_POValues = &helicopter_p3_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = helicopter_p3_P.HILInitialize_pwm_trailing_dead;
        }
      }

      result = hil_set_pwm_deadband(helicopter_p3_DW.HILInitialize_Card,
        helicopter_p3_P.HILInitialize_pwm_channels, 8U,
        &helicopter_p3_DW.HILInitialize_POSortedFreqs[0],
        &helicopter_p3_DW.HILInitialize_POValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p3_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_p3_P.HILInitialize_set_pwm_outputs_a && !is_switching) ||
        (helicopter_p3_P.HILInitialize_set_pwm_outputs_g && is_switching)) {
      {
        int_T i1;
        real_T *dw_POValues = &helicopter_p3_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = helicopter_p3_P.HILInitialize_initial_pwm_outpu;
        }
      }

      result = hil_write_pwm(helicopter_p3_DW.HILInitialize_Card,
        helicopter_p3_P.HILInitialize_pwm_channels, 8U,
        &helicopter_p3_DW.HILInitialize_POValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p3_M, _rt_error_message);
        return;
      }
    }

    if (helicopter_p3_P.HILInitialize_set_pwm_outputs_o) {
      {
        int_T i1;
        real_T *dw_POValues = &helicopter_p3_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = helicopter_p3_P.HILInitialize_watchdog_pwm_outp;
        }
      }

      result = hil_watchdog_set_pwm_expiration_state
        (helicopter_p3_DW.HILInitialize_Card,
         helicopter_p3_P.HILInitialize_pwm_channels, 8U,
         &helicopter_p3_DW.HILInitialize_POValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p3_M, _rt_error_message);
        return;
      }
    }
  }

  /* Start for S-Function (hil_read_encoder_timebase_block): '<S5>/HIL Read Encoder Timebase' */

  /* S-Function Block: helicopter_p3/Helicopter_interface/HIL Read Encoder Timebase (hil_read_encoder_timebase_block) */
  {
    t_error result;
    result = hil_task_create_encoder_reader(helicopter_p3_DW.HILInitialize_Card,
      helicopter_p3_P.HILReadEncoderTimebase_samples_,
      helicopter_p3_P.HILReadEncoderTimebase_channels, 3,
      &helicopter_p3_DW.HILReadEncoderTimebase_Task);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(helicopter_p3_M, _rt_error_message);
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
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      -0.52359877557010082, -0.5235987755642737, -0.523598775553429,
      -0.52359877554539536, -0.523598775534987, -0.52359877551809753,
      -0.52359877549416634, -0.52359877543823641, -0.52359877537700339,
      -0.52359877516553377, -0.5235987744075713, -0.38856151693505014,
      -0.10947154576632799, 0.11008507929539565, 0.27695896968908779,
      0.39795513806563371, 0.47968511189580049, 0.52317212167507288,
      0.5235975888594897, 0.52359766533353491, 0.523522272169707,
      0.50347393523763606, 0.46500607547487488, 0.42080544680520654,
      0.37350683555863434, 0.325254300687161, 0.27774421552174355,
      0.23227259445307866, 0.1897842523416865, 0.15092186790263568,
      0.11607345081574434, 0.085417096181567134, 0.058962244764886165,
      0.036586951701655879, 0.018070901997919352, 0.0031241017683262722,
      -0.0085886759700303152, -0.017427498416825095, -0.023760658261418224,
      -0.027951507046544741, -0.030348373829806388, -0.031277192136107856,
      -0.03103645878592037, -0.029894160250593469, -0.028086322790268559,
      -0.025816869230590671, -0.023258495673530191, -0.020554313921294035,
      -0.017820038464141749, -0.015146529393388624, -0.012602533674109637,
      -0.010237496211730516, -0.0080843386421720932, -0.0061621275089442928,
      -0.0044785743474293873, -0.0030323281756330376, -0.0018150360797909084,
      -0.00081316013657415612, -9.549032196946477E-6, 0.00061522934637608679,
      0.0010817821142279993, 0.00141099589635491, 0.0016233323515493657,
      0.0017382938399858085, 0.0017740377732728134, 0.0017471183876366521,
      0.0016723354911031354, 0.0015626709934849882, 0.0014292955951053568,
      0.0012816297693176043, 0.0011274450280714945, 0.00097299333079889187,
      0.00082315432348978876, 0.00068159183083307839, 0.00055091263670487619,
      0.000432822055372892, 0.00032827210528230593, 0.00023759924470379755,
      0.00016064961560442007, 9.6890575478778267E-5, 4.5507986914326361E-5,
      5.4892945212776972E-6, -2.4307136191623909E-5, -4.5095596381838941E-5,
      -5.8118438511728273E-5, -6.4612009594695559E-5, -6.5781640734996052E-5,
      -6.27840546694942E-5, -5.67153621871943E-5, -4.8602563972649892E-5,
      -3.9396114705153972E-5, -2.99606078919878E-5, -2.1060012853229062E-5,
      -1.3333279218625669E-5, -7.2559895976796354E-6, -3.0853331651053157E-6,
      -7.9196466760515511E-7, 2.9049271196480229E-13, 2.8223049138088423E-13,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 } ;

    helicopter_p3_DW._PWORK.TimePtr = (void *) pTimeValues0;
    helicopter_p3_DW._PWORK.DataPtr = (void *) pDataValues0;
    helicopter_p3_DW._IWORK.PrevIndex = 0;
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
      2.6335051105279454, 2.4931956060921214, 2.3345185761615084,
      2.1583785868490466, 1.9678013793910307, 1.7674143111840392,
      1.5625874980938677, 1.3586949815519547, 1.1606091748584673,
      0.97237558041863792, 0.79697339345539686, 0.63645596478416511,
      0.49218023370945141, 0.36487408959506545, 0.25465219321775506,
      0.16110387797438538, 0.08340392809500384, 0.020423202350655624,
      -0.029171039238896534, -0.066828956411080637, -0.094046912933813162,
      -0.11231060497587368, -0.12305126577742866, -0.12761341876914017,
      -0.12723258646661931, -0.12302139842084625, -0.11596263112850518,
      -0.10690783149026067, -0.0965803092563445, -0.085581424264179945,
      -0.0743992352473636, -0.063418714305358739, -0.05293286161931577,
      -0.043154176408169369, -0.034226050896825393, -0.026233753290835136,
      -0.019214753017630832, -0.013168216831196764, -0.0080635681554018582,
      -0.0038480548927893772, -0.00045331367007891911, 0.0021990479275836587,
      0.0041928041520926116, 0.00561333362183495, 0.0065446340674039944,
      0.0070670446640132964, 0.0072555883992256537, 0.00717884734891728,
      0.0068982867461100314, 0.0064679486571853654, 0.0059344423489134015,
      0.00533716554639824, 0.00470869833854137, 0.0040753191533885423,
      0.0034575997404622419, 0.0028710432632298639, 0.002326736279307707,
      0.0018319914728920569, 0.0013909634467172427, 0.0010052246554293506,
      0.0006742926704059346, 0.00039610342959854529, 0.00016742798170867962,
      -1.5767470818722493E-5, -0.00015801663695189159, -0.00026410037795433934,
      -0.0003388422428808592, -0.00038694780217422516, -0.00041288294587281149,
      -0.00042078620319196079, -0.00041441021093319142, -0.00039708766735298062,
      -0.00037171741885080656, -0.00034076670716504711, -0.00030628602753693878,
      -0.00026993349082189352, -0.00023300602612681616, -0.00019647519006521155,
      -0.00016102575186731618, -0.00012709559024953539, -9.4915759362575934E-5,
      -6.4549848886681226E-5, -3.5931968247110295E-5, -8.90281586304268E-6,
      1.6756662629216607E-5, 4.1294584513793983E-5, 6.4961146184283784E-5,
      8.7986382401692974E-5, 0.00011056407427959991, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.015002048902967588,
      -0.046506351610164379, -0.0916250137011204, -0.14672753933980562,
      -0.20869397116710753, -0.27519058107821581, -0.34459405525876891,
      -0.41582470126375731, -0.48818590983236315, -0.56123801773799742,
      -0.6347081197171518, -0.70455995724454534, -0.76230882982676307,
      -0.80154827282266494, -0.81930725235538626, -0.81557006616235084,
      -0.79234322676864843, -0.752934377754017, -0.701608747847663,
      -0.64206971467962648, -0.577102924293554, -0.509224576452243,
      -0.4408875855039408, -0.37419326096817779, -0.31079979951222536,
      -0.251922902972092, -0.19837696635290777, -0.15063166868343555,
      -0.1088718260856292, -0.073054768162941236, -0.042962643200919033,
      -0.018248611961545185, 0.0015233292153843511, 0.016844752188393083,
      0.028235069174665135, 0.036219198558278862, 0.041310088940965571,
      0.043995539973959032, 0.044728756072566288, 0.043922083773320263,
      0.041943410749472748, 0.039114740849886455, 0.035712502050676768,
      0.031969190429261905, 0.028076001098118079, 0.024186144751037138,
      0.020418594708480491, 0.016862053055750791, 0.013578964896142697,
      0.010609446395951174, 0.0079750249033366787, 0.0056821178842702156,
      0.0037252017875770457, 0.0020896423917380735, 0.00075417494615029389,
      -0.00030696419593263016, -0.0011222424059281297, -0.0017213523503977969,
      -0.00213402522778699, -0.0023891072047597826, -0.0025138688261266123,
      -0.0025335167353104458, -0.0024708776464043374, -0.0023462259036286463,
      -0.0021772279303877615, -0.001978979220361735, -0.001764112099398392,
      -0.0015429551598507023, -0.0013237279347927996, -0.001112756957928692,
      -0.00091470178625859759, -0.00073278180480874336, -0.00056899665923181128,
      -0.00042433495870892578, -0.00029896745440521427, -0.00019242223187259866,
      -0.00010374056949348027, -3.1613023975731933E-5, 2.5503974335942615E-5,
      6.9290179621708263E-5, 0.00010148099930956148, 0.00012380285204390278,
      0.00013792272381329832, 0.00014541015216104617, 0.00014770986408117455,
      0.0001461233495472836, 0.00014179775809244663, 0.00013572065177198833,
      0.00012871932884870295, 0.00012146364720444398, 0.00011447152785914882,
      0.0001081166148371356, 0.00010263791926990227, 9.8151692839174612E-5,
      9.4666251982824368E-5, 9.2100950170501852E-5, 9.0310772812492854E-5,
      8.9117581556552312E-5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, -0.10602875205272408, -0.22266037930984584, -0.31888147179445075,
      -0.38944360628061003, -0.43795507373668463, -0.46997264225288332,
      -0.49051724871039321, -0.50343100132617824, -0.51142138573774532,
      -0.51630439838137154, -0.51925862085137386, -0.493686109605494,
      -0.4081469757761842, -0.27732939664082307, -0.1255136847705694,
      0.026413004694513489, 0.16415842996596167, 0.27852669368166977,
      0.36274994972918, 0.42079914709343369, 0.4591604621240658,
      0.47973823822633, 0.48297975254009523, 0.47137001370869425,
      0.44804077413680832, 0.41611941829275922, 0.37844222958880369,
      0.33744552889087281, 0.29514261842853851, 0.25314128603218705,
      0.21267964635096109, 0.17466933393808798, 0.1397405288756205,
      0.10828596596649624, 0.080502410232849334, 0.056428777159355216,
      0.035980468895922678, 0.018979742265402912, 0.0051820913529894022,
      -0.0057012517239213483, -0.013984505231128571, -0.019991958515317061,
      -0.024045724438581249, -0.026456296882301139, -0.02751557529288981,
      -0.027492019034477309, -0.026627604785715738, -0.025136278075419967,
      -0.023203613224271655, -0.020987422631045478, -0.018619084962815051,
      -0.016205391096080726, -0.013830735579477095, -0.011559509151421603,
      -0.009438573859126392, -0.0074997261822996875, -0.0057620750143015818,
      -0.0042342802732423506, -0.0029166142877602385, -0.001802821991239294,
      -0.00088176749018458128, -0.00013886391784128369, 0.00044270915734417031,
      0.00088099091082270188, 0.0011944131308961052, 0.0014011461669582706,
      0.0015185987485064106, 0.0015630527840328849, 0.0015494142990218191,
      0.0014910622900408681, 0.0013997783117753323, 0.0012857409497022785,
      0.0011575708558671505, 0.0010224136499126248, 0.00088604963993681163,
      0.00075302094104493444, 0.00062676812038820937, 0.00050976994481345662,
      0.00040368113003726397, 0.00030946417626937238, 0.00022751241941018833,
      0.00015776233017028078, 9.979386123692303E-5, 5.291828421678302E-5,
      1.6253485815667129E-5, -1.1212878521268874E-5, -3.057162808147177E-5,
      -4.2950666207507692E-5, -4.9482676176520095E-5, -5.1280386470862627E-5,
      -4.941762880234524E-5, -4.4914097873271347E-5, -3.8721327550667935E-5,
      -3.1706934740841842E-5, -2.46337645243791E-5, -1.8130573183646177E-5,
      -1.265228957857616E-5, -8.433020089556033E-6, -5.4490044438406734E-6, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.42411500820670567,
      -0.46652650902318626, -0.38488436993311886, -0.28224853793933624,
      -0.19404586981899727, -0.12807027405949384, -0.08217842582473886,
      -0.051655010457839094, -0.031961537640967354, -0.019532050569203839,
      -0.011816889874708669, 0.1022900449888203, 0.34215653532253992,
      0.52327031654674538, 0.60726284748631565, 0.60770675786563233,
      0.55098170109109368, 0.45747305486813322, 0.33689302419534184,
      0.23219678946231573, 0.1534452601278293, 0.082311104414357614,
      0.012966057260361865, -0.046438955320303024, -0.0933169582822429,
      -0.1276854233708955, -0.15070875481052126, -0.16398680278642264,
      -0.16921164184403639, -0.16800532958010495, -0.16184655871960296,
      -0.15204124964619167, -0.13971522024456909, -0.12581825163119612,
      -0.11113422292928676, -0.096294532288675572, -0.081793233048429281,
      -0.0680029065167782, -0.055190603644353173, -0.043533372302342138,
      -0.033133014023528022, -0.0240298131314531, -0.016215063687755889,
      -0.0096422897695787012, -0.0042371136370538114, 9.4225038950877178E-5,
      0.0034576570003471566, 0.0059653068464839447, 0.0077306594098940995,
      0.0088647623782055762, 0.0094733506782225686, 0.0096547754722381574,
      0.009498622071715394, 0.00908490571752283, 0.0084837411744817064,
      0.0077553907126076873, 0.00695060467729329, 0.0061111789695377878,
      0.0052706639472293116, 0.0044551691913846435, 0.0036842180095197161,
      0.0029716142946740556, 0.002326292306042681, 0.0017531270192149915,
      0.0012536888855944786, 0.00082693214954952636, 0.00046981033149342573,
      0.00017781614740676252, -5.4553934743398818E-5, -0.00023340803062293867,
      -0.00036513590776127819, -0.00045614944299135019, -0.00051268037003964666,
      -0.00054062881851723807, -0.0005454560346023873, -0.00053211479026664366,
      -0.00050501127732603527, -0.00046799269699814591, -0.00042435525380390526,
      -0.00037686780977070136, -0.000327807022135871, -0.00027900035165876516,
      -0.00023187387043256582, -0.00018750230277969493, -0.00014665918830359843,
      -0.00010986545204687889, -7.7434992939946456E-5, -4.9516147203278563E-5,
      -2.6128034575184497E-5, -7.1908358765050205E-6, 7.4510359749346684E-6,
      1.8014129017160688E-5, 2.4771086591278778E-5, 2.805757654016949E-5,
      2.829268616671608E-5, 2.6012770663796804E-5, 2.1913139721145188E-5,
      1.687708325694563E-5, 1.1936067883726561E-5, 8.0243530609130433E-6, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0 } ;

    helicopter_p3_DW._PWORK_k.TimePtr = (void *) pTimeValues0;
    helicopter_p3_DW._PWORK_k.DataPtr = (void *) pDataValues0;
    helicopter_p3_DW._IWORK_g.PrevIndex = 0;
  }

  /* Start for ToFile: '<Root>/To File3' */
  {
    char fileName[509] = "p_ref_practical.mat";
    FILE *fp = (NULL);
    if ((fp = fopen(fileName, "wb")) == (NULL)) {
      rtmSetErrorStatus(helicopter_p3_M,
                        "Error creating .mat file p_ref_practical.mat");
      return;
    }

    if (rt_WriteMat4FileHeader(fp,2,0,"p_ref")) {
      rtmSetErrorStatus(helicopter_p3_M,
                        "Error writing mat file header to file p_ref_practical.mat");
      return;
    }

    helicopter_p3_DW.ToFile3_IWORK.Count = 0;
    helicopter_p3_DW.ToFile3_IWORK.Decimation = -1;
    helicopter_p3_DW.ToFile3_PWORK.FilePtr = fp;
  }

  /* Start for ToFile: '<S8>/To File' */
  {
    char fileName[509] = "p_dot_practical.mat";
    FILE *fp = (NULL);
    if ((fp = fopen(fileName, "wb")) == (NULL)) {
      rtmSetErrorStatus(helicopter_p3_M,
                        "Error creating .mat file p_dot_practical.mat");
      return;
    }

    if (rt_WriteMat4FileHeader(fp,2,0,"p_dot")) {
      rtmSetErrorStatus(helicopter_p3_M,
                        "Error writing mat file header to file p_dot_practical.mat");
      return;
    }

    helicopter_p3_DW.ToFile_IWORK.Count = 0;
    helicopter_p3_DW.ToFile_IWORK.Decimation = -1;
    helicopter_p3_DW.ToFile_PWORK.FilePtr = fp;
  }

  /* Start for ToFile: '<S8>/To File1' */
  {
    char fileName[509] = "p_practical.mat";
    FILE *fp = (NULL);
    if ((fp = fopen(fileName, "wb")) == (NULL)) {
      rtmSetErrorStatus(helicopter_p3_M,
                        "Error creating .mat file p_practical.mat");
      return;
    }

    if (rt_WriteMat4FileHeader(fp,2,0,"p")) {
      rtmSetErrorStatus(helicopter_p3_M,
                        "Error writing mat file header to file p_practical.mat");
      return;
    }

    helicopter_p3_DW.ToFile1_IWORK.Count = 0;
    helicopter_p3_DW.ToFile1_IWORK.Decimation = -1;
    helicopter_p3_DW.ToFile1_PWORK.FilePtr = fp;
  }

  /* Start for ToFile: '<S8>/To File2' */
  {
    char fileName[509] = "lambda_practical.mat";
    FILE *fp = (NULL);
    if ((fp = fopen(fileName, "wb")) == (NULL)) {
      rtmSetErrorStatus(helicopter_p3_M,
                        "Error creating .mat file lambda_practical.mat");
      return;
    }

    if (rt_WriteMat4FileHeader(fp,2,0,"lambda")) {
      rtmSetErrorStatus(helicopter_p3_M,
                        "Error writing mat file header to file lambda_practical.mat");
      return;
    }

    helicopter_p3_DW.ToFile2_IWORK.Count = 0;
    helicopter_p3_DW.ToFile2_IWORK.Decimation = -1;
    helicopter_p3_DW.ToFile2_PWORK.FilePtr = fp;
  }

  /* Start for ToFile: '<S8>/To File4' */
  {
    char fileName[509] = "r_practical.mat";
    FILE *fp = (NULL);
    if ((fp = fopen(fileName, "wb")) == (NULL)) {
      rtmSetErrorStatus(helicopter_p3_M,
                        "Error creating .mat file r_practical.mat");
      return;
    }

    if (rt_WriteMat4FileHeader(fp,2,0,"r")) {
      rtmSetErrorStatus(helicopter_p3_M,
                        "Error writing mat file header to file r_practical.mat");
      return;
    }

    helicopter_p3_DW.ToFile4_IWORK.Count = 0;
    helicopter_p3_DW.ToFile4_IWORK.Decimation = -1;
    helicopter_p3_DW.ToFile4_PWORK.FilePtr = fp;
  }

  /* InitializeConditions for TransferFcn: '<S5>/Travel: Transfer Fcn' */
  helicopter_p3_X.TravelTransferFcn_CSTATE = 0.0;

  /* InitializeConditions for TransferFcn: '<S5>/Pitch: Transfer Fcn' */
  helicopter_p3_X.PitchTransferFcn_CSTATE = 0.0;

  /* InitializeConditions for TransferFcn: '<S5>/Elevation: Transfer Fcn' */
  helicopter_p3_X.ElevationTransferFcn_CSTATE = 0.0;

  /* InitializeConditions for Integrator: '<S4>/Integrator' */
  helicopter_p3_X.Integrator_CSTATE = helicopter_p3_P.Integrator_IC;

  /* InitializeConditions for Derivative: '<S5>/Derivative' */
  helicopter_p3_DW.TimeStampA = (rtInf);
  helicopter_p3_DW.TimeStampB = (rtInf);
}

/* Model terminate function */
void helicopter_p3_terminate(void)
{
  /* Terminate for S-Function (hil_initialize_block): '<Root>/HIL Initialize' */

  /* S-Function Block: helicopter_p3/HIL Initialize (hil_initialize_block) */
  {
    t_boolean is_switching;
    t_int result;
    t_uint32 num_final_analog_outputs = 0;
    t_uint32 num_final_pwm_outputs = 0;
    hil_task_stop_all(helicopter_p3_DW.HILInitialize_Card);
    hil_monitor_stop_all(helicopter_p3_DW.HILInitialize_Card);
    is_switching = false;
    if ((helicopter_p3_P.HILInitialize_set_analog_out_ex && !is_switching) ||
        (helicopter_p3_P.HILInitialize_set_analog_outp_c && is_switching)) {
      {
        int_T i1;
        real_T *dw_AOVoltages = &helicopter_p3_DW.HILInitialize_AOVoltages[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOVoltages[i1] = helicopter_p3_P.HILInitialize_final_analog_outp;
        }
      }

      num_final_analog_outputs = 8U;
    }

    if ((helicopter_p3_P.HILInitialize_set_pwm_output_ap && !is_switching) ||
        (helicopter_p3_P.HILInitialize_set_pwm_outputs_p && is_switching)) {
      {
        int_T i1;
        real_T *dw_POValues = &helicopter_p3_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = helicopter_p3_P.HILInitialize_final_pwm_outputs;
        }
      }

      num_final_pwm_outputs = 8U;
    }

    if (0
        || num_final_analog_outputs > 0
        || num_final_pwm_outputs > 0
        ) {
      /* Attempt to write the final outputs atomically (due to firmware issue in old Q2-USB). Otherwise write channels individually */
      result = hil_write(helicopter_p3_DW.HILInitialize_Card
                         , helicopter_p3_P.HILInitialize_analog_output_cha,
                         num_final_analog_outputs
                         , helicopter_p3_P.HILInitialize_pwm_channels,
                         num_final_pwm_outputs
                         , NULL, 0
                         , NULL, 0
                         , &helicopter_p3_DW.HILInitialize_AOVoltages[0]
                         , &helicopter_p3_DW.HILInitialize_POValues[0]
                         , (t_boolean *) NULL
                         , NULL
                         );
      if (result == -QERR_HIL_WRITE_NOT_SUPPORTED) {
        t_error local_result;
        result = 0;

        /* The hil_write operation is not supported by this card. Write final outputs for each channel type */
        if (num_final_analog_outputs > 0) {
          local_result = hil_write_analog(helicopter_p3_DW.HILInitialize_Card,
            helicopter_p3_P.HILInitialize_analog_output_cha,
            num_final_analog_outputs,
            &helicopter_p3_DW.HILInitialize_AOVoltages[0]);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (num_final_pwm_outputs > 0) {
          local_result = hil_write_pwm(helicopter_p3_DW.HILInitialize_Card,
            helicopter_p3_P.HILInitialize_pwm_channels, num_final_pwm_outputs,
            &helicopter_p3_DW.HILInitialize_POValues[0]);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (result < 0) {
          msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
            (_rt_error_message));
          rtmSetErrorStatus(helicopter_p3_M, _rt_error_message);
        }
      }
    }

    hil_task_delete_all(helicopter_p3_DW.HILInitialize_Card);
    hil_monitor_delete_all(helicopter_p3_DW.HILInitialize_Card);
    hil_close(helicopter_p3_DW.HILInitialize_Card);
    helicopter_p3_DW.HILInitialize_Card = NULL;
  }

  /* Terminate for ToFile: '<Root>/To File3' */
  {
    FILE *fp = (FILE *) helicopter_p3_DW.ToFile3_PWORK.FilePtr;
    if (fp != (NULL)) {
      char fileName[509] = "p_ref_practical.mat";
      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_p3_M,
                          "Error closing MAT-file p_ref_practical.mat");
        return;
      }

      if ((fp = fopen(fileName, "r+b")) == (NULL)) {
        rtmSetErrorStatus(helicopter_p3_M,
                          "Error reopening MAT-file p_ref_practical.mat");
        return;
      }

      if (rt_WriteMat4FileHeader(fp, 2, helicopter_p3_DW.ToFile3_IWORK.Count,
           "p_ref")) {
        rtmSetErrorStatus(helicopter_p3_M,
                          "Error writing header for p_ref to MAT-file p_ref_practical.mat");
      }

      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_p3_M,
                          "Error closing MAT-file p_ref_practical.mat");
        return;
      }

      helicopter_p3_DW.ToFile3_PWORK.FilePtr = (NULL);
    }
  }

  /* Terminate for ToFile: '<S8>/To File' */
  {
    FILE *fp = (FILE *) helicopter_p3_DW.ToFile_PWORK.FilePtr;
    if (fp != (NULL)) {
      char fileName[509] = "p_dot_practical.mat";
      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_p3_M,
                          "Error closing MAT-file p_dot_practical.mat");
        return;
      }

      if ((fp = fopen(fileName, "r+b")) == (NULL)) {
        rtmSetErrorStatus(helicopter_p3_M,
                          "Error reopening MAT-file p_dot_practical.mat");
        return;
      }

      if (rt_WriteMat4FileHeader(fp, 2, helicopter_p3_DW.ToFile_IWORK.Count,
           "p_dot")) {
        rtmSetErrorStatus(helicopter_p3_M,
                          "Error writing header for p_dot to MAT-file p_dot_practical.mat");
      }

      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_p3_M,
                          "Error closing MAT-file p_dot_practical.mat");
        return;
      }

      helicopter_p3_DW.ToFile_PWORK.FilePtr = (NULL);
    }
  }

  /* Terminate for ToFile: '<S8>/To File1' */
  {
    FILE *fp = (FILE *) helicopter_p3_DW.ToFile1_PWORK.FilePtr;
    if (fp != (NULL)) {
      char fileName[509] = "p_practical.mat";
      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_p3_M,
                          "Error closing MAT-file p_practical.mat");
        return;
      }

      if ((fp = fopen(fileName, "r+b")) == (NULL)) {
        rtmSetErrorStatus(helicopter_p3_M,
                          "Error reopening MAT-file p_practical.mat");
        return;
      }

      if (rt_WriteMat4FileHeader(fp, 2, helicopter_p3_DW.ToFile1_IWORK.Count,
           "p")) {
        rtmSetErrorStatus(helicopter_p3_M,
                          "Error writing header for p to MAT-file p_practical.mat");
      }

      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_p3_M,
                          "Error closing MAT-file p_practical.mat");
        return;
      }

      helicopter_p3_DW.ToFile1_PWORK.FilePtr = (NULL);
    }
  }

  /* Terminate for ToFile: '<S8>/To File2' */
  {
    FILE *fp = (FILE *) helicopter_p3_DW.ToFile2_PWORK.FilePtr;
    if (fp != (NULL)) {
      char fileName[509] = "lambda_practical.mat";
      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_p3_M,
                          "Error closing MAT-file lambda_practical.mat");
        return;
      }

      if ((fp = fopen(fileName, "r+b")) == (NULL)) {
        rtmSetErrorStatus(helicopter_p3_M,
                          "Error reopening MAT-file lambda_practical.mat");
        return;
      }

      if (rt_WriteMat4FileHeader(fp, 2, helicopter_p3_DW.ToFile2_IWORK.Count,
           "lambda")) {
        rtmSetErrorStatus(helicopter_p3_M,
                          "Error writing header for lambda to MAT-file lambda_practical.mat");
      }

      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_p3_M,
                          "Error closing MAT-file lambda_practical.mat");
        return;
      }

      helicopter_p3_DW.ToFile2_PWORK.FilePtr = (NULL);
    }
  }

  /* Terminate for ToFile: '<S8>/To File4' */
  {
    FILE *fp = (FILE *) helicopter_p3_DW.ToFile4_PWORK.FilePtr;
    if (fp != (NULL)) {
      char fileName[509] = "r_practical.mat";
      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_p3_M,
                          "Error closing MAT-file r_practical.mat");
        return;
      }

      if ((fp = fopen(fileName, "r+b")) == (NULL)) {
        rtmSetErrorStatus(helicopter_p3_M,
                          "Error reopening MAT-file r_practical.mat");
        return;
      }

      if (rt_WriteMat4FileHeader(fp, 2, helicopter_p3_DW.ToFile4_IWORK.Count,
           "r")) {
        rtmSetErrorStatus(helicopter_p3_M,
                          "Error writing header for r to MAT-file r_practical.mat");
      }

      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_p3_M,
                          "Error closing MAT-file r_practical.mat");
        return;
      }

      helicopter_p3_DW.ToFile4_PWORK.FilePtr = (NULL);
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
  helicopter_p3_output();
  UNUSED_PARAMETER(tid);
}

void MdlUpdate(int_T tid)
{
  helicopter_p3_update();
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
  helicopter_p3_initialize();
}

void MdlTerminate(void)
{
  helicopter_p3_terminate();
}

/* Registration function */
RT_MODEL_helicopter_p3_T *helicopter_p3(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* non-finite (run-time) assignments */
  helicopter_p3_P.Integrator_UpperSat = rtInf;
  helicopter_p3_P.Integrator_LowerSat = rtMinusInf;

  /* initialize real-time model */
  (void) memset((void *)helicopter_p3_M, 0,
                sizeof(RT_MODEL_helicopter_p3_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&helicopter_p3_M->solverInfo,
                          &helicopter_p3_M->Timing.simTimeStep);
    rtsiSetTPtr(&helicopter_p3_M->solverInfo, &rtmGetTPtr(helicopter_p3_M));
    rtsiSetStepSizePtr(&helicopter_p3_M->solverInfo,
                       &helicopter_p3_M->Timing.stepSize0);
    rtsiSetdXPtr(&helicopter_p3_M->solverInfo,
                 &helicopter_p3_M->ModelData.derivs);
    rtsiSetContStatesPtr(&helicopter_p3_M->solverInfo, (real_T **)
                         &helicopter_p3_M->ModelData.contStates);
    rtsiSetNumContStatesPtr(&helicopter_p3_M->solverInfo,
      &helicopter_p3_M->Sizes.numContStates);
    rtsiSetErrorStatusPtr(&helicopter_p3_M->solverInfo, (&rtmGetErrorStatus
      (helicopter_p3_M)));
    rtsiSetRTModelPtr(&helicopter_p3_M->solverInfo, helicopter_p3_M);
  }

  rtsiSetSimTimeStep(&helicopter_p3_M->solverInfo, MAJOR_TIME_STEP);
  helicopter_p3_M->ModelData.intgData.f[0] = helicopter_p3_M->ModelData.odeF[0];
  helicopter_p3_M->ModelData.contStates = ((real_T *) &helicopter_p3_X);
  rtsiSetSolverData(&helicopter_p3_M->solverInfo, (void *)
                    &helicopter_p3_M->ModelData.intgData);
  rtsiSetSolverName(&helicopter_p3_M->solverInfo,"ode1");

  /* Initialize timing info */
  {
    int_T *mdlTsMap = helicopter_p3_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    mdlTsMap[1] = 1;
    helicopter_p3_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    helicopter_p3_M->Timing.sampleTimes =
      (&helicopter_p3_M->Timing.sampleTimesArray[0]);
    helicopter_p3_M->Timing.offsetTimes =
      (&helicopter_p3_M->Timing.offsetTimesArray[0]);

    /* task periods */
    helicopter_p3_M->Timing.sampleTimes[0] = (0.0);
    helicopter_p3_M->Timing.sampleTimes[1] = (0.002);

    /* task offsets */
    helicopter_p3_M->Timing.offsetTimes[0] = (0.0);
    helicopter_p3_M->Timing.offsetTimes[1] = (0.0);
  }

  rtmSetTPtr(helicopter_p3_M, &helicopter_p3_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits = helicopter_p3_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    mdlSampleHits[1] = 1;
    helicopter_p3_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(helicopter_p3_M, 35.0);
  helicopter_p3_M->Timing.stepSize0 = 0.002;
  helicopter_p3_M->Timing.stepSize1 = 0.002;

  /* External mode info */
  helicopter_p3_M->Sizes.checksums[0] = (1059362091U);
  helicopter_p3_M->Sizes.checksums[1] = (2022201324U);
  helicopter_p3_M->Sizes.checksums[2] = (2525720794U);
  helicopter_p3_M->Sizes.checksums[3] = (2252730363U);

  {
    static const sysRanDType rtAlwaysEnabled = SUBSYS_RAN_BC_ENABLE;
    static RTWExtModeInfo rt_ExtModeInfo;
    static const sysRanDType *systemRan[1];
    helicopter_p3_M->extModeInfo = (&rt_ExtModeInfo);
    rteiSetSubSystemActiveVectorAddresses(&rt_ExtModeInfo, systemRan);
    systemRan[0] = &rtAlwaysEnabled;
    rteiSetModelMappingInfoPtr(helicopter_p3_M->extModeInfo,
      &helicopter_p3_M->SpecialInfo.mappingInfo);
    rteiSetChecksumsPtr(helicopter_p3_M->extModeInfo,
                        helicopter_p3_M->Sizes.checksums);
    rteiSetTPtr(helicopter_p3_M->extModeInfo, rtmGetTPtr(helicopter_p3_M));
  }

  helicopter_p3_M->solverInfoPtr = (&helicopter_p3_M->solverInfo);
  helicopter_p3_M->Timing.stepSize = (0.002);
  rtsiSetFixedStepSize(&helicopter_p3_M->solverInfo, 0.002);
  rtsiSetSolverMode(&helicopter_p3_M->solverInfo, SOLVER_MODE_SINGLETASKING);

  /* block I/O */
  helicopter_p3_M->ModelData.blockIO = ((void *) &helicopter_p3_B);

  {
    int_T i;
    for (i = 0; i < 6; i++) {
      helicopter_p3_B.Gain1[i] = 0.0;
    }

    helicopter_p3_B.ElevationCounttorad = 0.0;
    helicopter_p3_B.Gain = 0.0;
    helicopter_p3_B.Sum = 0.0;
    helicopter_p3_B.PitchCounttorad = 0.0;
    helicopter_p3_B.Gain_i = 0.0;
    helicopter_p3_B.Gain_g = 0.0;
    helicopter_p3_B.Gain_p = 0.0;
    helicopter_p3_B.Sum3 = 0.0;
    helicopter_p3_B.Gain_d = 0.0;
    helicopter_p3_B.Gain_b = 0.0;
    helicopter_p3_B.Gain_dg = 0.0;
    helicopter_p3_B.Sum5 = 0.0;
    helicopter_p3_B.Sum1 = 0.0;
    helicopter_p3_B.Gain1_f = 0.0;
    helicopter_p3_B.Sum2 = 0.0;
    helicopter_p3_B.K_ei = 0.0;
    helicopter_p3_B.Gain_l = 0.0;
    helicopter_p3_B.BackmotorSaturation = 0.0;
    helicopter_p3_B.FrontmotorSaturation = 0.0;
  }

  /* parameters */
  helicopter_p3_M->ModelData.defaultParam = ((real_T *)&helicopter_p3_P);

  /* states (continuous) */
  {
    real_T *x = (real_T *) &helicopter_p3_X;
    helicopter_p3_M->ModelData.contStates = (x);
    (void) memset((void *)&helicopter_p3_X, 0,
                  sizeof(X_helicopter_p3_T));
  }

  /* states (dwork) */
  helicopter_p3_M->ModelData.dwork = ((void *) &helicopter_p3_DW);
  (void) memset((void *)&helicopter_p3_DW, 0,
                sizeof(DW_helicopter_p3_T));

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      helicopter_p3_DW.HILInitialize_AIMinimums[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      helicopter_p3_DW.HILInitialize_AIMaximums[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      helicopter_p3_DW.HILInitialize_AOMinimums[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      helicopter_p3_DW.HILInitialize_AOMaximums[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      helicopter_p3_DW.HILInitialize_AOVoltages[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      helicopter_p3_DW.HILInitialize_FilterFrequency[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      helicopter_p3_DW.HILInitialize_POSortedFreqs[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      helicopter_p3_DW.HILInitialize_POValues[i] = 0.0;
    }
  }

  helicopter_p3_DW.TimeStampA = 0.0;
  helicopter_p3_DW.LastUAtTimeA = 0.0;
  helicopter_p3_DW.TimeStampB = 0.0;
  helicopter_p3_DW.LastUAtTimeB = 0.0;
  helicopter_p3_DW.HILWriteAnalog_Buffer[0] = 0.0;
  helicopter_p3_DW.HILWriteAnalog_Buffer[1] = 0.0;

  /* data type transition information */
  {
    static DataTypeTransInfo dtInfo;
    (void) memset((char_T *) &dtInfo, 0,
                  sizeof(dtInfo));
    helicopter_p3_M->SpecialInfo.mappingInfo = (&dtInfo);
    dtInfo.numDataTypes = 16;
    dtInfo.dataTypeSizes = &rtDataTypeSizes[0];
    dtInfo.dataTypeNames = &rtDataTypeNames[0];

    /* Block I/O transition table */
    dtInfo.B = &rtBTransTable;

    /* Parameters transition table */
    dtInfo.P = &rtPTransTable;
  }

  /* Initialize Sizes */
  helicopter_p3_M->Sizes.numContStates = (4);/* Number of continuous states */
  helicopter_p3_M->Sizes.numY = (0);   /* Number of model outputs */
  helicopter_p3_M->Sizes.numU = (0);   /* Number of model inputs */
  helicopter_p3_M->Sizes.sysDirFeedThru = (0);/* The model is not direct feedthrough */
  helicopter_p3_M->Sizes.numSampTimes = (2);/* Number of sample times */
  helicopter_p3_M->Sizes.numBlocks = (71);/* Number of blocks */
  helicopter_p3_M->Sizes.numBlockIO = (20);/* Number of block outputs */
  helicopter_p3_M->Sizes.numBlockPrms = (148);/* Sum of parameter "widths" */
  return helicopter_p3_M;
}

/*========================================================================*
 * End of Classic call interface                                          *
 *========================================================================*/
