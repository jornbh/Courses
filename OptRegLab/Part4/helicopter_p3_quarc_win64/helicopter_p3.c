/*
 * helicopter_p3.c
 *
 * Code generation for model "helicopter_p3".
 *
 * Model version              : 1.207
 * Simulink Coder version : 8.6 (R2014a) 27-Dec-2013
 * C source code generated on : Wed Feb 21 12:11:44 2018
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
  real_T rtb_Backgain;
  real_T rtb_HILReadEncoderTimebase_o1;
  real_T rtb_HILReadEncoderTimebase_o2;
  real_T rtb_HILReadEncoderTimebase_o3;
  real_T *lastU;
  real_T rtb_Derivative;
  real_T tmp[6];
  int32_T i;
  int32_T i_0;
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
    /* S-Function (hil_read_encoder_timebase_block): '<S4>/HIL Read Encoder Timebase' */

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

    /* Gain: '<S4>/Elevation: Count to rad' */
    helicopter_p3_B.ElevationCounttorad =
      helicopter_p3_P.ElevationCounttorad_Gain * rtb_HILReadEncoderTimebase_o3;

    /* Gain: '<S8>/Gain' */
    helicopter_p3_B.Gain = helicopter_p3_P.Gain_Gain *
      helicopter_p3_B.ElevationCounttorad;

    /* Sum: '<Root>/Sum' incorporates:
     *  Constant: '<Root>/elavation_offset [deg]'
     */
    helicopter_p3_B.Sum = helicopter_p3_B.Gain +
      helicopter_p3_P.elavation_offsetdeg_Value;

    /* Gain: '<S4>/Pitch: Count to rad' */
    helicopter_p3_B.PitchCounttorad = helicopter_p3_P.PitchCounttorad_Gain *
      rtb_HILReadEncoderTimebase_o2;

    /* Gain: '<S10>/Gain' */
    helicopter_p3_B.Gain_i = helicopter_p3_P.Gain_Gain_a *
      helicopter_p3_B.PitchCounttorad;

    /* Gain: '<S4>/Travel: Count to rad' */
    helicopter_p3_B.TravelCounttorad = helicopter_p3_P.TravelCounttorad_Gain *
      rtb_HILReadEncoderTimebase_o1;

    /* Gain: '<S13>/Gain' */
    helicopter_p3_B.Gain_p = helicopter_p3_P.Gain_Gain_ar *
      helicopter_p3_B.TravelCounttorad;

    /* Sum: '<Root>/Sum3' incorporates:
     *  Constant: '<Root>/Travel offset [deg]'
     */
    helicopter_p3_B.Sum3 = helicopter_p3_B.Gain_p +
      helicopter_p3_P.Traveloffsetdeg_Value;
  }

  /* TransferFcn: '<S4>/Travel: Transfer Fcn' */
  rtb_Backgain = 0.0;
  rtb_Backgain += helicopter_p3_P.TravelTransferFcn_C *
    helicopter_p3_X.TravelTransferFcn_CSTATE;
  rtb_Backgain += helicopter_p3_P.TravelTransferFcn_D *
    helicopter_p3_B.TravelCounttorad;

  /* Gain: '<S14>/Gain' */
  helicopter_p3_B.Gain_d = helicopter_p3_P.Gain_Gain_l * rtb_Backgain;

  /* TransferFcn: '<S4>/Pitch: Transfer Fcn' */
  rtb_Backgain = 0.0;
  rtb_Backgain += helicopter_p3_P.PitchTransferFcn_C *
    helicopter_p3_X.PitchTransferFcn_CSTATE;
  rtb_Backgain += helicopter_p3_P.PitchTransferFcn_D *
    helicopter_p3_B.PitchCounttorad;

  /* Gain: '<S11>/Gain' */
  helicopter_p3_B.Gain_b = helicopter_p3_P.Gain_Gain_ae * rtb_Backgain;

  /* TransferFcn: '<S4>/Elevation: Transfer Fcn' */
  rtb_Backgain = 0.0;
  rtb_Backgain += helicopter_p3_P.ElevationTransferFcn_C *
    helicopter_p3_X.ElevationTransferFcn_CSTATE;
  rtb_Backgain += helicopter_p3_P.ElevationTransferFcn_D *
    helicopter_p3_B.ElevationCounttorad;

  /* Gain: '<S9>/Gain' */
  helicopter_p3_B.Gain_dg = helicopter_p3_P.Gain_Gain_n * rtb_Backgain;

  /* Gain: '<S2>/Gain1' */
  helicopter_p3_B.Gain1[0] = helicopter_p3_P.Gain1_Gain * helicopter_p3_B.Sum3;
  helicopter_p3_B.Gain1[1] = helicopter_p3_P.Gain1_Gain * helicopter_p3_B.Gain_d;
  helicopter_p3_B.Gain1[2] = helicopter_p3_P.Gain1_Gain * helicopter_p3_B.Gain_i;
  helicopter_p3_B.Gain1[3] = helicopter_p3_P.Gain1_Gain * helicopter_p3_B.Gain_b;
  helicopter_p3_B.Gain1[4] = helicopter_p3_P.Gain1_Gain * helicopter_p3_B.Sum;
  helicopter_p3_B.Gain1[5] = helicopter_p3_P.Gain1_Gain *
    helicopter_p3_B.Gain_dg;
  if (rtmIsMajorTimeStep(helicopter_p3_M)) {
    /* ToFile: '<S7>/To File' */
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

    /* ToFile: '<S7>/To File1' */
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

    /* ToFile: '<S7>/To File2' */
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

    /* ToFile: '<S7>/To File4' */
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

  /* FromWorkspace: '<Root>/     ' */
  {
    real_T *pDataValues = (real_T *) helicopter_p3_DW._PWORK.DataPtr;
    real_T *pTimeValues = (real_T *) helicopter_p3_DW._PWORK.TimePtr;
    int_T currTimeIndex = helicopter_p3_DW._IWORK.PrevIndex;
    real_T t = helicopter_p3_M->Timing.t[0];

    /* Get index */
    if (t <= pTimeValues[0]) {
      currTimeIndex = 0;
    } else if (t >= pTimeValues[80]) {
      currTimeIndex = 79;
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
          {
            int_T elIdx;
            for (elIdx = 0; elIdx < 2; ++elIdx) {
              (&helicopter_p3_B.u[0])[elIdx] = pDataValues[currTimeIndex];
              pDataValues += 81;
            }
          }
        } else {
          {
            int_T elIdx;
            for (elIdx = 0; elIdx < 2; ++elIdx) {
              (&helicopter_p3_B.u[0])[elIdx] = pDataValues[currTimeIndex + 1];
              pDataValues += 81;
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
          for (elIdx = 0; elIdx < 2; ++elIdx) {
            d1 = pDataValues[TimeIndex];
            d2 = pDataValues[TimeIndex + 1];
            (&helicopter_p3_B.u[0])[elIdx] = (real_T) rtInterpolate(d1, d2, f1,
              f2);
            pDataValues += 81;
          }
        }
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
    } else if (t >= pTimeValues[80]) {
      currTimeIndex = 79;
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
            for (elIdx = 0; elIdx < 6; ++elIdx) {
              (&helicopter_p3_B.u_p[0])[elIdx] = pDataValues[currTimeIndex];
              pDataValues += 81;
            }
          }
        } else {
          {
            int_T elIdx;
            for (elIdx = 0; elIdx < 6; ++elIdx) {
              (&helicopter_p3_B.u_p[0])[elIdx] = pDataValues[currTimeIndex + 1];
              pDataValues += 81;
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
          for (elIdx = 0; elIdx < 6; ++elIdx) {
            d1 = pDataValues[TimeIndex];
            d2 = pDataValues[TimeIndex + 1];
            (&helicopter_p3_B.u_p[0])[elIdx] = (real_T) rtInterpolate(d1, d2, f1,
              f2);
            pDataValues += 81;
          }
        }
      }
    }
  }

  /* Sum: '<S5>/Sum4' incorporates:
   *  Gain: '<S5>/Gain'
   */
  for (i = 0; i < 6; i++) {
    tmp[i] = helicopter_p3_B.Gain1[i] - helicopter_p3_B.u_p[i];
  }

  /* End of Sum: '<S5>/Sum4' */

  /* Sum: '<S5>/Sum5' incorporates:
   *  Gain: '<S5>/Gain'
   */
  for (i = 0; i < 2; i++) {
    rtb_Derivative = 0.0;
    for (i_0 = 0; i_0 < 6; i_0++) {
      rtb_Derivative += helicopter_p3_P.K[(i_0 << 1) + i] * tmp[i_0];
    }

    helicopter_p3_B.Sum5[i] = helicopter_p3_B.u[i] - rtb_Derivative;
  }

  /* End of Sum: '<S5>/Sum5' */

  /* Sum: '<Root>/Sum1' incorporates:
   *  Constant: '<Root>/Vd_bias'
   *  Gain: '<S6>/K_pd'
   *  Gain: '<S6>/K_pp'
   *  Sum: '<S6>/Sum2'
   *  Sum: '<S6>/Sum3'
   */
  helicopter_p3_B.Sum1 = ((helicopter_p3_B.Sum5[0] - helicopter_p3_B.Gain1[2]) *
    helicopter_p3_P.K_pp - helicopter_p3_P.K_pd * helicopter_p3_B.Gain1[3]) +
    helicopter_p3_P.Vd_ff;
  if (rtmIsMajorTimeStep(helicopter_p3_M)) {
  }

  /* Integrator: '<S3>/Integrator'
   *
   * Regarding '<S3>/Integrator':
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

  /* Sum: '<S3>/Sum' */
  rtb_Derivative = helicopter_p3_B.Sum5[1] - helicopter_p3_B.Gain1[4];

  /* Sum: '<Root>/Sum2' incorporates:
   *  Constant: '<Root>/Vs_bias'
   *  Gain: '<S3>/K_ed'
   *  Gain: '<S3>/K_ep'
   *  Sum: '<S3>/Sum1'
   */
  helicopter_p3_B.Sum2 = ((helicopter_p3_P.K_ep * rtb_Derivative + rtb_Backgain)
    - helicopter_p3_P.K_ed * helicopter_p3_B.Gain1[5]) + helicopter_p3_P.Vs_ff;
  if (rtmIsMajorTimeStep(helicopter_p3_M)) {
  }

  /* Gain: '<S1>/Back gain' incorporates:
   *  Sum: '<S1>/Subtract'
   */
  rtb_Backgain = (helicopter_p3_B.Sum2 - helicopter_p3_B.Sum1) *
    helicopter_p3_P.Backgain_Gain;

  /* Gain: '<S3>/K_ei' */
  helicopter_p3_B.K_ei = helicopter_p3_P.K_ei * rtb_Derivative;
  if (rtmIsMajorTimeStep(helicopter_p3_M)) {
  }

  /* Derivative: '<S4>/Derivative' */
  if ((helicopter_p3_DW.TimeStampA >= helicopter_p3_M->Timing.t[0]) &&
      (helicopter_p3_DW.TimeStampB >= helicopter_p3_M->Timing.t[0])) {
    rtb_Derivative = 0.0;
  } else {
    rtb_Derivative = helicopter_p3_DW.TimeStampA;
    lastU = &helicopter_p3_DW.LastUAtTimeA;
    if (helicopter_p3_DW.TimeStampA < helicopter_p3_DW.TimeStampB) {
      if (helicopter_p3_DW.TimeStampB < helicopter_p3_M->Timing.t[0]) {
        rtb_Derivative = helicopter_p3_DW.TimeStampB;
        lastU = &helicopter_p3_DW.LastUAtTimeB;
      }
    } else {
      if (helicopter_p3_DW.TimeStampA >= helicopter_p3_M->Timing.t[0]) {
        rtb_Derivative = helicopter_p3_DW.TimeStampB;
        lastU = &helicopter_p3_DW.LastUAtTimeB;
      }
    }

    rtb_Derivative = (helicopter_p3_B.PitchCounttorad - *lastU) /
      (helicopter_p3_M->Timing.t[0] - rtb_Derivative);
  }

  /* End of Derivative: '<S4>/Derivative' */

  /* Gain: '<S12>/Gain' */
  helicopter_p3_B.Gain_l = helicopter_p3_P.Gain_Gain_a1 * rtb_Derivative;
  if (rtmIsMajorTimeStep(helicopter_p3_M)) {
  }

  /* Saturate: '<S4>/Back motor: Saturation' */
  if (rtb_Backgain > helicopter_p3_P.BackmotorSaturation_UpperSat) {
    helicopter_p3_B.BackmotorSaturation =
      helicopter_p3_P.BackmotorSaturation_UpperSat;
  } else if (rtb_Backgain < helicopter_p3_P.BackmotorSaturation_LowerSat) {
    helicopter_p3_B.BackmotorSaturation =
      helicopter_p3_P.BackmotorSaturation_LowerSat;
  } else {
    helicopter_p3_B.BackmotorSaturation = rtb_Backgain;
  }

  /* End of Saturate: '<S4>/Back motor: Saturation' */
  if (rtmIsMajorTimeStep(helicopter_p3_M)) {
  }

  /* Gain: '<S1>/Front gain' incorporates:
   *  Sum: '<S1>/Add'
   */
  rtb_Derivative = (helicopter_p3_B.Sum1 + helicopter_p3_B.Sum2) *
    helicopter_p3_P.Frontgain_Gain;

  /* Saturate: '<S4>/Front motor: Saturation' */
  if (rtb_Derivative > helicopter_p3_P.FrontmotorSaturation_UpperSat) {
    helicopter_p3_B.FrontmotorSaturation =
      helicopter_p3_P.FrontmotorSaturation_UpperSat;
  } else if (rtb_Derivative < helicopter_p3_P.FrontmotorSaturation_LowerSat) {
    helicopter_p3_B.FrontmotorSaturation =
      helicopter_p3_P.FrontmotorSaturation_LowerSat;
  } else {
    helicopter_p3_B.FrontmotorSaturation = rtb_Derivative;
  }

  /* End of Saturate: '<S4>/Front motor: Saturation' */
  if (rtmIsMajorTimeStep(helicopter_p3_M)) {
    /* S-Function (hil_write_analog_block): '<S4>/HIL Write Analog' */

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

    /* ToFile: '<S5>/To File3' */
    {
      if (!(++helicopter_p3_DW.ToFile3_IWORK.Decimation % 1) &&
          (helicopter_p3_DW.ToFile3_IWORK.Count*3)+1 < 100000000 ) {
        FILE *fp = (FILE *) helicopter_p3_DW.ToFile3_PWORK.FilePtr;
        if (fp != (NULL)) {
          real_T u[3];
          helicopter_p3_DW.ToFile3_IWORK.Decimation = 0;
          u[0] = helicopter_p3_M->Timing.t[1];
          u[1] = helicopter_p3_B.Sum5[0];
          u[2] = helicopter_p3_B.Sum5[1];
          if (fwrite(u, sizeof(real_T), 3, fp) != 3) {
            rtmSetErrorStatus(helicopter_p3_M,
                              "Error writing to MAT-file p_ref_practical.mat");
            return;
          }

          if (((++helicopter_p3_DW.ToFile3_IWORK.Count)*3)+1 >= 100000000) {
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
}

/* Model update function */
void helicopter_p3_update(void)
{
  real_T *lastU;

  /* Update for Derivative: '<S4>/Derivative' */
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

  /* End of Update for Derivative: '<S4>/Derivative' */
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

  /* Derivatives for TransferFcn: '<S4>/Travel: Transfer Fcn' */
  _rtXdot->TravelTransferFcn_CSTATE = 0.0;
  _rtXdot->TravelTransferFcn_CSTATE += helicopter_p3_P.TravelTransferFcn_A *
    helicopter_p3_X.TravelTransferFcn_CSTATE;
  _rtXdot->TravelTransferFcn_CSTATE += helicopter_p3_B.TravelCounttorad;

  /* Derivatives for TransferFcn: '<S4>/Pitch: Transfer Fcn' */
  _rtXdot->PitchTransferFcn_CSTATE = 0.0;
  _rtXdot->PitchTransferFcn_CSTATE += helicopter_p3_P.PitchTransferFcn_A *
    helicopter_p3_X.PitchTransferFcn_CSTATE;
  _rtXdot->PitchTransferFcn_CSTATE += helicopter_p3_B.PitchCounttorad;

  /* Derivatives for TransferFcn: '<S4>/Elevation: Transfer Fcn' */
  _rtXdot->ElevationTransferFcn_CSTATE = 0.0;
  _rtXdot->ElevationTransferFcn_CSTATE += helicopter_p3_P.ElevationTransferFcn_A
    * helicopter_p3_X.ElevationTransferFcn_CSTATE;
  _rtXdot->ElevationTransferFcn_CSTATE += helicopter_p3_B.ElevationCounttorad;

  /* Derivatives for Integrator: '<S3>/Integrator' */
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

  /* Start for S-Function (hil_read_encoder_timebase_block): '<S4>/HIL Read Encoder Timebase' */

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

  /* Start for ToFile: '<S7>/To File' */
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

  /* Start for ToFile: '<S7>/To File1' */
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

  /* Start for ToFile: '<S7>/To File2' */
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

  /* Start for ToFile: '<S7>/To File4' */
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

  /* Start for FromWorkspace: '<Root>/     ' */
  {
    static real_T pTimeValues0[] = { 0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75,
      2.0, 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 3.75, 4.0, 4.25, 4.5, 4.75, 5.0,
      5.25, 5.5, 5.75, 6.0, 6.25, 6.5, 6.75, 7.0, 7.25, 7.5, 7.75, 8.0, 8.25,
      8.5, 8.75, 9.0, 9.25, 9.5, 9.75, 10.0, 10.25, 10.5, 10.75, 11.0, 11.25,
      11.5, 11.75, 12.0, 12.25, 12.5, 12.75, 13.0, 13.25, 13.5, 13.75, 14.0,
      14.25, 14.5, 14.75, 15.0, 15.25, 15.5, 15.75, 16.0, 16.25, 16.5, 16.75,
      17.0, 17.25, 17.5, 17.75, 18.0, 18.25, 18.5, 18.75, 19.0, 19.25, 19.5,
      19.75, 20.0 } ;

    static real_T pDataValues0[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.52359877559829882,
      0.52359877559829882, 0.52359877559829882, 0.52359877559829882,
      0.52359877559829882, 0.52359877559829882, 0.52359877559829882,
      0.52359877559829882, 0.52359877559829882, 0.52359877559829882,
      0.52359877559829882, 0.36412961256260129, 0.11143374518898416,
      -0.10676753688746815, -0.27223084477368964, -0.39175446454460572,
      -0.47193438456018766, -0.51906212963589227, -0.52359877559829882,
      -0.52359877559829882, -0.51834129423177211, -0.48689202863153097,
      -0.44654720623277039, -0.40051861842340264, -0.35151025666701974,
      -0.30175366811119825, -0.25305667539111504, -0.2068472516555295,
      -0.16421909287380251, -0.12597912501944619, -0.092684681844571473,
      -0.064675491027681775, -0.042084143450785726, -0.02484827614462936,
      -0.012693942178289803, -0.0050967998537812653, -0.0012421186038879491,
      -1.389836654707772E-7, 1.3574620645637292E-7, 2.2106409622104953E-7,
      2.2106409622104953E-7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.28552362121336305, 0.30300426313541495, 0.3196536279129476,
      0.33487443163833308, 0.34792126447612187, 0.35786305149129988,
      0.3635435588394999, 0.36354299460155043, 0.35612264921660342,
      0.3391653988008303, 0.31009409675018668, 0.26579404948323704,
      0.20250931431468491, 0.11571982554963731, -3.7213512190177079E-7,
      -1.4185029722204478E-6, -1.4695423054645363E-6, -2.2638544729446362E-6,
      -1.3150222551042533E-6, -1.7474956604876095E-7, -4.4680390503847306E-7,
      -8.881959508552795E-7, 1.3583258349172207E-7, 6.9102456121622578E-7,
      -4.9160611400833335E-7, -5.1099766678187469E-7, -2.8102625526220158E-7,
      -8.9081878800706593E-7, -1.2110977261884866E-6, 4.3069320311056064E-7,
      -1.0355567869345776E-6, -5.6023281159195815E-7, -9.7711764267096336E-7,
      5.5404010051833196E-8, 3.5246746447602758E-8, 1.3057929843338921E-8,
      -1.4564036970886603E-7, -4.2766671030125076E-7, -8.9406243691078779E-7,
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
      19.75, 20.0 } ;

    static real_T pDataValues0[] = { 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1378421413625261, 3.1262155534579983,
      3.1033093000299643, 3.0666274151911783, 3.0144539223941584,
      2.9456562771175667, 2.8595077632935446, 2.7555515879651526,
      2.633505110490284, 2.4931956060320961, 2.334518576064299,
      2.1585535912274869, 1.9683298380759535, 1.7684158167314468,
      1.5641057539157648, 1.3606974695344898, 1.1629913867163155,
      0.975002954096943, 0.7997295329506815, 0.63925354746697172,
      0.49492130338877405, 0.36735853417632808, 0.25655730117395575,
      0.16198988698244618, 0.08272715942582419, 0.017550303631900573,
      -0.034949128434622771, -0.076282733938807412, -0.10799255763223041,
      -0.13159150160451472, -0.14851577607264335, -0.16008818784674744,
      -0.16749119153675177, -0.17174879782165911, -0.17371678682411973,
      -0.17408117140474252, -0.17336530311493439, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.015002048909068422,
      -0.046506351618112111, -0.091625013712135384, -0.14672753935514371,
      -0.20869397118807925, -0.27519058110636674, -0.34459405529608839,
      -0.41582470131356869, -0.48818590989947463, -0.56123801783275185,
      -0.63470811987118858, -0.70385993934724767, -0.7608950126061349,
      -0.79965608537802713, -0.81724025126272737, -0.8136331375251,
      -0.79082433127269725, -0.75195373047749037, -0.70109368458504617,
      -0.64190394193483913, -0.57732897631279079, -0.510251076849784,
      -0.44320493200948907, -0.37826965676603841, -0.3170509102264879,
      -0.26070742317569451, -0.20999772826609339, -0.16533442201673854,
      -0.12683929477369202, -0.094395775889137154, -0.067697097872514447,
      -0.046289647096416468, -0.029612014760017267, -0.017030425139629372,
      -0.0078719560098423833, -0.0014575383224911551, 0.0028634731592325421,
      0.0056762284208374493, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.1060287520586555, 0.22266037932317656, 0.31888147181640641,
      0.38944360631144165, 0.43795507377677839, 0.46997264230390062,
      0.4905172487754707, 0.5034310014147434, 0.5114213858602934,
      0.51630439857701838, 0.51925862127063693, 0.48873864937207934,
      0.40310211477568891, 0.27394845859849154, 0.12427816867202264,
      -0.025493702257101427, -0.1612039313795364, -0.274722560835985,
      -0.35945937973592923, -0.41833049511529224, -0.45639119433863673,
      -0.47408097479800948, -0.47385654525250887, -0.4589377251321009,
      -0.43267071968149856, -0.39821424758970553, -0.35839675641172358,
      -0.31566319061725473, -0.27206885717137885, -0.22929840054323503,
      -0.18869605937674216, -0.15129968608319333, -0.11787113577883226,
      -0.088921870235836961, -0.064728561977427573, -0.04533465439922036,
      -0.030539258858257864, -0.019879479932511747, -0.012629274632533112, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.424115008234622,
      0.46652650905808418, 0.38488436997291947, 0.28224853798014093,
      0.19404586986134689, 0.12807027410848898, 0.082178425886280423,
      0.051655010557090618, 0.031961537782199859, 0.019532050866899828,
      0.011816890774474396, -0.12207988759423034, -0.34254613838556169,
      -0.5166146247087896, -0.59868115970587554, -0.59908748371649623,
      -0.54284091648973987, -0.45407451782579461, -0.338947275599777,
      -0.23548446151745189, -0.15224279689337791, -0.070759121837491182,
      0.00089771818200263262, 0.059675280481631829, 0.1050680218024094,
      0.13782588836717208, 0.15926996471192784, 0.17093426317787533,
      0.17437733378350359, 0.17108182651257531, 0.1624093646659715,
      0.14958549317419528, 0.13371420121744426, 0.11579706217198123,
      0.096773233033637523, 0.077575630312828853, 0.059181582163849972,
      0.042639115702984454, 0.029000821199914539, 0.019002639927243905, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0044613065814587977,
      0.012541728129043755, 0.023526924310536986, 0.036802269938989558,
      0.05182744072541625, 0.068112893526991111, 0.08519754747370481,
      0.10262713326303008, 0.11993252731975682, 0.13660748326133001,
      0.15208497473986038, 0.16571163344597545, 0.17671950778141837,
      0.1841942915346203, 0.18703913122582577, 0.18629470302489251,
      0.18281387248719053, 0.17729235947642391, 0.17029473741351389,
      0.16227632501905029, 0.15360165346980542, 0.14456006835138735,
      0.13537885579949221, 0.12623420611483921, 0.11726041654813686,
      0.10855765691822718, 0.10019838879619505, 0.092232710396280088,
      0.084692832848001331, 0.0775967953164317, 0.070951425473929658,
      0.0647549394115962, 0.058998943574352786, 0.053670151633801691,
      0.048751699735769524, 0.044224264896997406, 0.040066941183916141,
      0.036257937577797258, 0.032775124947483823, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.017845226325835191, 0.032321686190339829,
      0.043940784725972924, 0.053101382513810275, 0.060100683145706761,
      0.065141811206299471, 0.068338615786854823, 0.069718343157301077,
      0.069221576226906972, 0.066699823766292743, 0.061909965914121415,
      0.054506634824460255, 0.044031497341771725, 0.029899135012807658,
      0.011379358764821977, -0.0029777128037330484, -0.013923322150807989,
      -0.022086052043066337, -0.027990488251640105, -0.032073649577854448,
      -0.034698686196979515, -0.036166340473672211, -0.036724850207580526,
      -0.03657859873861203, -0.035895158266809411, -0.034811038519638689,
      -0.033437072488128529, -0.031862713599659843, -0.030159510193114959,
      -0.028384150126278533, -0.026581479370008165, -0.024785944249333832,
      -0.023023983348973643, -0.021315167762204369, -0.019673807592128674,
      -0.018109739355088496, -0.016629294852325071, -0.015236014424475537,
      -0.013931250521253716, -0.012714558989552616, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 } ;

    helicopter_p3_DW._PWORK_k.TimePtr = (void *) pTimeValues0;
    helicopter_p3_DW._PWORK_k.DataPtr = (void *) pDataValues0;
    helicopter_p3_DW._IWORK_g.PrevIndex = 0;
  }

  /* Start for ToFile: '<S5>/To File3' */
  {
    char fileName[509] = "p_ref_practical.mat";
    FILE *fp = (NULL);
    if ((fp = fopen(fileName, "wb")) == (NULL)) {
      rtmSetErrorStatus(helicopter_p3_M,
                        "Error creating .mat file p_ref_practical.mat");
      return;
    }

    if (rt_WriteMat4FileHeader(fp,3,0,"p_ref")) {
      rtmSetErrorStatus(helicopter_p3_M,
                        "Error writing mat file header to file p_ref_practical.mat");
      return;
    }

    helicopter_p3_DW.ToFile3_IWORK.Count = 0;
    helicopter_p3_DW.ToFile3_IWORK.Decimation = -1;
    helicopter_p3_DW.ToFile3_PWORK.FilePtr = fp;
  }

  /* InitializeConditions for TransferFcn: '<S4>/Travel: Transfer Fcn' */
  helicopter_p3_X.TravelTransferFcn_CSTATE = 0.0;

  /* InitializeConditions for TransferFcn: '<S4>/Pitch: Transfer Fcn' */
  helicopter_p3_X.PitchTransferFcn_CSTATE = 0.0;

  /* InitializeConditions for TransferFcn: '<S4>/Elevation: Transfer Fcn' */
  helicopter_p3_X.ElevationTransferFcn_CSTATE = 0.0;

  /* InitializeConditions for Integrator: '<S3>/Integrator' */
  helicopter_p3_X.Integrator_CSTATE = helicopter_p3_P.Integrator_IC;

  /* InitializeConditions for Derivative: '<S4>/Derivative' */
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

  /* Terminate for ToFile: '<S7>/To File' */
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

  /* Terminate for ToFile: '<S7>/To File1' */
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

  /* Terminate for ToFile: '<S7>/To File2' */
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

  /* Terminate for ToFile: '<S7>/To File4' */
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

  /* Terminate for ToFile: '<S5>/To File3' */
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

      if (rt_WriteMat4FileHeader(fp, 3, helicopter_p3_DW.ToFile3_IWORK.Count,
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
  helicopter_p3_M->Sizes.checksums[0] = (3711407808U);
  helicopter_p3_M->Sizes.checksums[1] = (2282610858U);
  helicopter_p3_M->Sizes.checksums[2] = (3706267791U);
  helicopter_p3_M->Sizes.checksums[3] = (1628814615U);

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

    for (i = 0; i < 6; i++) {
      helicopter_p3_B.u_p[i] = 0.0;
    }

    helicopter_p3_B.ElevationCounttorad = 0.0;
    helicopter_p3_B.Gain = 0.0;
    helicopter_p3_B.Sum = 0.0;
    helicopter_p3_B.PitchCounttorad = 0.0;
    helicopter_p3_B.Gain_i = 0.0;
    helicopter_p3_B.TravelCounttorad = 0.0;
    helicopter_p3_B.Gain_p = 0.0;
    helicopter_p3_B.Sum3 = 0.0;
    helicopter_p3_B.Gain_d = 0.0;
    helicopter_p3_B.Gain_b = 0.0;
    helicopter_p3_B.Gain_dg = 0.0;
    helicopter_p3_B.u[0] = 0.0;
    helicopter_p3_B.u[1] = 0.0;
    helicopter_p3_B.Sum5[0] = 0.0;
    helicopter_p3_B.Sum5[1] = 0.0;
    helicopter_p3_B.Sum1 = 0.0;
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
  helicopter_p3_M->Sizes.numBlocks = (70);/* Number of blocks */
  helicopter_p3_M->Sizes.numBlockIO = (21);/* Number of block outputs */
  helicopter_p3_M->Sizes.numBlockPrms = (153);/* Sum of parameter "widths" */
  return helicopter_p3_M;
}

/*========================================================================*
 * End of Classic call interface                                          *
 *========================================================================*/
