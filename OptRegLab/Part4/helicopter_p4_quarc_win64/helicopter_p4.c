/*
 * helicopter_p4.c
 *
 * Code generation for model "helicopter_p4".
 *
 * Model version              : 1.213
 * Simulink Coder version : 8.6 (R2014a) 27-Dec-2013
 * C source code generated on : Wed Feb 28 12:09:25 2018
 *
 * Target selection: quarc_win64.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: 32-bit Generic
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */
#include "helicopter_p4.h"
#include "helicopter_p4_private.h"
#include "helicopter_p4_dt.h"

/* Block signals (auto storage) */
B_helicopter_p4_T helicopter_p4_B;

/* Continuous states */
X_helicopter_p4_T helicopter_p4_X;

/* Block states (auto storage) */
DW_helicopter_p4_T helicopter_p4_DW;

/* Real-time model */
RT_MODEL_helicopter_p4_T helicopter_p4_M_;
RT_MODEL_helicopter_p4_T *const helicopter_p4_M = &helicopter_p4_M_;

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
  helicopter_p4_derivatives();
  rtsiSetT(si, tnew);
  for (i = 0; i < nXc; i++) {
    *x += h * f0[i];
    x++;
  }

  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

/* Model output function */
void helicopter_p4_output(void)
{
  /* local block i/o variables */
  real_T rtb_u[2];
  real_T rtb_Sum4[6];
  real_T rtb_Backgain;
  real_T rtb_HILReadEncoderTimebase_o1;
  real_T rtb_HILReadEncoderTimebase_o2;
  real_T rtb_HILReadEncoderTimebase_o3;
  real_T *lastU;
  real_T rtb_Derivative;
  int32_T i;
  int32_T i_0;
  if (rtmIsMajorTimeStep(helicopter_p4_M)) {
    /* set solver stop time */
    if (!(helicopter_p4_M->Timing.clockTick0+1)) {
      rtsiSetSolverStopTime(&helicopter_p4_M->solverInfo,
                            ((helicopter_p4_M->Timing.clockTickH0 + 1) *
        helicopter_p4_M->Timing.stepSize0 * 4294967296.0));
    } else {
      rtsiSetSolverStopTime(&helicopter_p4_M->solverInfo,
                            ((helicopter_p4_M->Timing.clockTick0 + 1) *
        helicopter_p4_M->Timing.stepSize0 + helicopter_p4_M->Timing.clockTickH0 *
        helicopter_p4_M->Timing.stepSize0 * 4294967296.0));
    }
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(helicopter_p4_M)) {
    helicopter_p4_M->Timing.t[0] = rtsiGetT(&helicopter_p4_M->solverInfo);
  }

  if (rtmIsMajorTimeStep(helicopter_p4_M)) {
    /* S-Function (hil_read_encoder_timebase_block): '<S4>/HIL Read Encoder Timebase' */

    /* S-Function Block: helicopter_p4/Helicopter_interface/HIL Read Encoder Timebase (hil_read_encoder_timebase_block) */
    {
      t_error result;
      result = hil_task_read_encoder
        (helicopter_p4_DW.HILReadEncoderTimebase_Task, 1,
         &helicopter_p4_DW.HILReadEncoderTimebase_Buffer[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p4_M, _rt_error_message);
      } else {
        rtb_HILReadEncoderTimebase_o1 =
          helicopter_p4_DW.HILReadEncoderTimebase_Buffer[0];
        rtb_HILReadEncoderTimebase_o2 =
          helicopter_p4_DW.HILReadEncoderTimebase_Buffer[1];
        rtb_HILReadEncoderTimebase_o3 =
          helicopter_p4_DW.HILReadEncoderTimebase_Buffer[2];
      }
    }

    /* Gain: '<S4>/Elevation: Count to rad' */
    helicopter_p4_B.ElevationCounttorad =
      helicopter_p4_P.ElevationCounttorad_Gain * rtb_HILReadEncoderTimebase_o3;

    /* Gain: '<S8>/Gain' */
    helicopter_p4_B.Gain = helicopter_p4_P.Gain_Gain *
      helicopter_p4_B.ElevationCounttorad;

    /* Sum: '<Root>/Sum' incorporates:
     *  Constant: '<Root>/elavation_offset [deg]'
     */
    helicopter_p4_B.Sum = helicopter_p4_B.Gain +
      helicopter_p4_P.elavation_offsetdeg_Value;

    /* Gain: '<S4>/Pitch: Count to rad' */
    helicopter_p4_B.PitchCounttorad = helicopter_p4_P.PitchCounttorad_Gain *
      rtb_HILReadEncoderTimebase_o2;

    /* Gain: '<S10>/Gain' */
    helicopter_p4_B.Gain_i = helicopter_p4_P.Gain_Gain_a *
      helicopter_p4_B.PitchCounttorad;

    /* Gain: '<S4>/Gain' incorporates:
     *  Gain: '<S4>/Travel: Count to rad'
     */
    helicopter_p4_B.Gain_g = helicopter_p4_P.TravelCounttorad_Gain *
      rtb_HILReadEncoderTimebase_o1 * helicopter_p4_P.Gain_Gain_n;

    /* Gain: '<S13>/Gain' */
    helicopter_p4_B.Gain_p = helicopter_p4_P.Gain_Gain_ar *
      helicopter_p4_B.Gain_g;

    /* Sum: '<Root>/Sum3' incorporates:
     *  Constant: '<Root>/Travel offset [deg]'
     */
    helicopter_p4_B.Sum3 = helicopter_p4_B.Gain_p +
      helicopter_p4_P.Traveloffsetdeg_Value;
  }

  /* TransferFcn: '<S4>/Travel: Transfer Fcn' */
  rtb_Backgain = 0.0;
  rtb_Backgain += helicopter_p4_P.TravelTransferFcn_C *
    helicopter_p4_X.TravelTransferFcn_CSTATE;
  rtb_Backgain += helicopter_p4_P.TravelTransferFcn_D * helicopter_p4_B.Gain_g;

  /* Gain: '<S14>/Gain' */
  helicopter_p4_B.Gain_d = helicopter_p4_P.Gain_Gain_l * rtb_Backgain;

  /* TransferFcn: '<S4>/Pitch: Transfer Fcn' */
  rtb_Backgain = 0.0;
  rtb_Backgain += helicopter_p4_P.PitchTransferFcn_C *
    helicopter_p4_X.PitchTransferFcn_CSTATE;
  rtb_Backgain += helicopter_p4_P.PitchTransferFcn_D *
    helicopter_p4_B.PitchCounttorad;

  /* Gain: '<S11>/Gain' */
  helicopter_p4_B.Gain_b = helicopter_p4_P.Gain_Gain_ae * rtb_Backgain;

  /* TransferFcn: '<S4>/Elevation: Transfer Fcn' */
  rtb_Backgain = 0.0;
  rtb_Backgain += helicopter_p4_P.ElevationTransferFcn_C *
    helicopter_p4_X.ElevationTransferFcn_CSTATE;
  rtb_Backgain += helicopter_p4_P.ElevationTransferFcn_D *
    helicopter_p4_B.ElevationCounttorad;

  /* Gain: '<S9>/Gain' */
  helicopter_p4_B.Gain_dg = helicopter_p4_P.Gain_Gain_nm * rtb_Backgain;

  /* Gain: '<S2>/Gain1' */
  helicopter_p4_B.Gain1[0] = helicopter_p4_P.Gain1_Gain * helicopter_p4_B.Sum3;
  helicopter_p4_B.Gain1[1] = helicopter_p4_P.Gain1_Gain * helicopter_p4_B.Gain_d;
  helicopter_p4_B.Gain1[2] = helicopter_p4_P.Gain1_Gain * helicopter_p4_B.Gain_i;
  helicopter_p4_B.Gain1[3] = helicopter_p4_P.Gain1_Gain * helicopter_p4_B.Gain_b;
  helicopter_p4_B.Gain1[4] = helicopter_p4_P.Gain1_Gain * helicopter_p4_B.Sum;
  helicopter_p4_B.Gain1[5] = helicopter_p4_P.Gain1_Gain *
    helicopter_p4_B.Gain_dg;
  if (rtmIsMajorTimeStep(helicopter_p4_M)) {
    /* ToFile: '<S7>/To File' */
    {
      if (!(++helicopter_p4_DW.ToFile_IWORK.Decimation % 1) &&
          (helicopter_p4_DW.ToFile_IWORK.Count*2)+1 < 100000000 ) {
        FILE *fp = (FILE *) helicopter_p4_DW.ToFile_PWORK.FilePtr;
        if (fp != (NULL)) {
          real_T u[2];
          helicopter_p4_DW.ToFile_IWORK.Decimation = 0;
          u[0] = helicopter_p4_M->Timing.t[1];
          u[1] = helicopter_p4_B.Gain1[3];
          if (fwrite(u, sizeof(real_T), 2, fp) != 2) {
            rtmSetErrorStatus(helicopter_p4_M,
                              "Error writing to MAT-file p_dot_practical.mat");
            return;
          }

          if (((++helicopter_p4_DW.ToFile_IWORK.Count)*2)+1 >= 100000000) {
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
      if (!(++helicopter_p4_DW.ToFile1_IWORK.Decimation % 1) &&
          (helicopter_p4_DW.ToFile1_IWORK.Count*2)+1 < 100000000 ) {
        FILE *fp = (FILE *) helicopter_p4_DW.ToFile1_PWORK.FilePtr;
        if (fp != (NULL)) {
          real_T u[2];
          helicopter_p4_DW.ToFile1_IWORK.Decimation = 0;
          u[0] = helicopter_p4_M->Timing.t[1];
          u[1] = helicopter_p4_B.Gain1[2];
          if (fwrite(u, sizeof(real_T), 2, fp) != 2) {
            rtmSetErrorStatus(helicopter_p4_M,
                              "Error writing to MAT-file p_practical.mat");
            return;
          }

          if (((++helicopter_p4_DW.ToFile1_IWORK.Count)*2)+1 >= 100000000) {
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
      if (!(++helicopter_p4_DW.ToFile2_IWORK.Decimation % 1) &&
          (helicopter_p4_DW.ToFile2_IWORK.Count*2)+1 < 100000000 ) {
        FILE *fp = (FILE *) helicopter_p4_DW.ToFile2_PWORK.FilePtr;
        if (fp != (NULL)) {
          real_T u[2];
          helicopter_p4_DW.ToFile2_IWORK.Decimation = 0;
          u[0] = helicopter_p4_M->Timing.t[1];
          u[1] = helicopter_p4_B.Gain1[0];
          if (fwrite(u, sizeof(real_T), 2, fp) != 2) {
            rtmSetErrorStatus(helicopter_p4_M,
                              "Error writing to MAT-file lambda_practical.mat");
            return;
          }

          if (((++helicopter_p4_DW.ToFile2_IWORK.Count)*2)+1 >= 100000000) {
            (void)fprintf(stdout,
                          "*** The ToFile block will stop logging data before\n"
                          "    the simulation has ended, because it has reached\n"
                          "    the maximum number of elements (100000000)\n"
                          "    allowed in MAT-file lambda_practical.mat.\n");
          }
        }
      }
    }

    /* ToFile: '<S7>/To File3' */
    {
      if (!(++helicopter_p4_DW.ToFile3_IWORK.Decimation % 1) &&
          (helicopter_p4_DW.ToFile3_IWORK.Count*2)+1 < 100000000 ) {
        FILE *fp = (FILE *) helicopter_p4_DW.ToFile3_PWORK.FilePtr;
        if (fp != (NULL)) {
          real_T u[2];
          helicopter_p4_DW.ToFile3_IWORK.Decimation = 0;
          u[0] = helicopter_p4_M->Timing.t[1];
          u[1] = helicopter_p4_B.Gain1[5];
          if (fwrite(u, sizeof(real_T), 2, fp) != 2) {
            rtmSetErrorStatus(helicopter_p4_M,
                              "Error writing to MAT-file e_dot_practical.mat");
            return;
          }

          if (((++helicopter_p4_DW.ToFile3_IWORK.Count)*2)+1 >= 100000000) {
            (void)fprintf(stdout,
                          "*** The ToFile block will stop logging data before\n"
                          "    the simulation has ended, because it has reached\n"
                          "    the maximum number of elements (100000000)\n"
                          "    allowed in MAT-file e_dot_practical.mat.\n");
          }
        }
      }
    }

    /* ToFile: '<S7>/To File4' */
    {
      if (!(++helicopter_p4_DW.ToFile4_IWORK.Decimation % 1) &&
          (helicopter_p4_DW.ToFile4_IWORK.Count*2)+1 < 100000000 ) {
        FILE *fp = (FILE *) helicopter_p4_DW.ToFile4_PWORK.FilePtr;
        if (fp != (NULL)) {
          real_T u[2];
          helicopter_p4_DW.ToFile4_IWORK.Decimation = 0;
          u[0] = helicopter_p4_M->Timing.t[1];
          u[1] = helicopter_p4_B.Gain1[1];
          if (fwrite(u, sizeof(real_T), 2, fp) != 2) {
            rtmSetErrorStatus(helicopter_p4_M,
                              "Error writing to MAT-file r_practical.mat");
            return;
          }

          if (((++helicopter_p4_DW.ToFile4_IWORK.Count)*2)+1 >= 100000000) {
            (void)fprintf(stdout,
                          "*** The ToFile block will stop logging data before\n"
                          "    the simulation has ended, because it has reached\n"
                          "    the maximum number of elements (100000000)\n"
                          "    allowed in MAT-file r_practical.mat.\n");
          }
        }
      }
    }

    /* ToFile: '<S7>/To File5' */
    {
      if (!(++helicopter_p4_DW.ToFile5_IWORK.Decimation % 1) &&
          (helicopter_p4_DW.ToFile5_IWORK.Count*2)+1 < 100000000 ) {
        FILE *fp = (FILE *) helicopter_p4_DW.ToFile5_PWORK.FilePtr;
        if (fp != (NULL)) {
          real_T u[2];
          helicopter_p4_DW.ToFile5_IWORK.Decimation = 0;
          u[0] = helicopter_p4_M->Timing.t[1];
          u[1] = helicopter_p4_B.Gain1[4];
          if (fwrite(u, sizeof(real_T), 2, fp) != 2) {
            rtmSetErrorStatus(helicopter_p4_M,
                              "Error writing to MAT-file e_practical.mat");
            return;
          }

          if (((++helicopter_p4_DW.ToFile5_IWORK.Count)*2)+1 >= 100000000) {
            (void)fprintf(stdout,
                          "*** The ToFile block will stop logging data before\n"
                          "    the simulation has ended, because it has reached\n"
                          "    the maximum number of elements (100000000)\n"
                          "    allowed in MAT-file e_practical.mat.\n");
          }
        }
      }
    }
  }

  /* FromWorkspace: '<Root>/     ' */
  {
    real_T *pDataValues = (real_T *) helicopter_p4_DW._PWORK.DataPtr;
    real_T *pTimeValues = (real_T *) helicopter_p4_DW._PWORK.TimePtr;
    int_T currTimeIndex = helicopter_p4_DW._IWORK.PrevIndex;
    real_T t = helicopter_p4_M->Timing.t[0];

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

    helicopter_p4_DW._IWORK.PrevIndex = currTimeIndex;

    /* Post output */
    {
      real_T t1 = pTimeValues[currTimeIndex];
      real_T t2 = pTimeValues[currTimeIndex + 1];
      if (t1 == t2) {
        if (t < t1) {
          {
            int_T elIdx;
            for (elIdx = 0; elIdx < 2; ++elIdx) {
              (&rtb_u[0])[elIdx] = pDataValues[currTimeIndex];
              pDataValues += 81;
            }
          }
        } else {
          {
            int_T elIdx;
            for (elIdx = 0; elIdx < 2; ++elIdx) {
              (&rtb_u[0])[elIdx] = pDataValues[currTimeIndex + 1];
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
            (&rtb_u[0])[elIdx] = (real_T) rtInterpolate(d1, d2, f1, f2);
            pDataValues += 81;
          }
        }
      }
    }
  }

  /* FromWorkspace: '<Root>/  ' */
  {
    real_T *pDataValues = (real_T *) helicopter_p4_DW._PWORK_k.DataPtr;
    real_T *pTimeValues = (real_T *) helicopter_p4_DW._PWORK_k.TimePtr;
    int_T currTimeIndex = helicopter_p4_DW._IWORK_g.PrevIndex;
    real_T t = helicopter_p4_M->Timing.t[0];

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

    helicopter_p4_DW._IWORK_g.PrevIndex = currTimeIndex;

    /* Post output */
    {
      real_T t1 = pTimeValues[currTimeIndex];
      real_T t2 = pTimeValues[currTimeIndex + 1];
      if (t1 == t2) {
        if (t < t1) {
          {
            int_T elIdx;
            for (elIdx = 0; elIdx < 6; ++elIdx) {
              (&rtb_Sum4[0])[elIdx] = pDataValues[currTimeIndex];
              pDataValues += 81;
            }
          }
        } else {
          {
            int_T elIdx;
            for (elIdx = 0; elIdx < 6; ++elIdx) {
              (&rtb_Sum4[0])[elIdx] = pDataValues[currTimeIndex + 1];
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
            (&rtb_Sum4[0])[elIdx] = (real_T) rtInterpolate(d1, d2, f1, f2);
            pDataValues += 81;
          }
        }
      }
    }
  }

  /* Sum: '<S5>/Sum4' */
  for (i = 0; i < 6; i++) {
    rtb_Sum4[i] = helicopter_p4_B.Gain1[i] - rtb_Sum4[i];
  }

  /* End of Sum: '<S5>/Sum4' */

  /* Sum: '<S5>/Sum5' incorporates:
   *  Gain: '<S5>/Gain'
   */
  for (i = 0; i < 2; i++) {
    rtb_Derivative = 0.0;
    for (i_0 = 0; i_0 < 6; i_0++) {
      rtb_Derivative += helicopter_p4_P.K[(i_0 << 1) + i] * rtb_Sum4[i_0];
    }

    helicopter_p4_B.Sum5[i] = rtb_u[i] - rtb_Derivative;
  }

  /* End of Sum: '<S5>/Sum5' */

  /* Sum: '<Root>/Sum1' incorporates:
   *  Constant: '<Root>/Vd_bias'
   *  Gain: '<S6>/K_pd'
   *  Gain: '<S6>/K_pp'
   *  Sum: '<S6>/Sum2'
   *  Sum: '<S6>/Sum3'
   */
  helicopter_p4_B.Sum1 = ((helicopter_p4_B.Sum5[0] - helicopter_p4_B.Gain1[2]) *
    helicopter_p4_P.K_pp - helicopter_p4_P.K_pd * helicopter_p4_B.Gain1[3]) +
    helicopter_p4_P.Vd_ff;
  if (rtmIsMajorTimeStep(helicopter_p4_M)) {
  }

  /* Integrator: '<S3>/Integrator'
   *
   * Regarding '<S3>/Integrator':
   *  Limited Integrator
   */
  if (helicopter_p4_X.Integrator_CSTATE >= helicopter_p4_P.Integrator_UpperSat )
  {
    helicopter_p4_X.Integrator_CSTATE = helicopter_p4_P.Integrator_UpperSat;
  } else if (helicopter_p4_X.Integrator_CSTATE <=
             (helicopter_p4_P.Integrator_LowerSat) ) {
    helicopter_p4_X.Integrator_CSTATE = (helicopter_p4_P.Integrator_LowerSat);
  }

  rtb_Backgain = helicopter_p4_X.Integrator_CSTATE;

  /* Sum: '<S3>/Sum' */
  rtb_Derivative = helicopter_p4_B.Sum5[1] - helicopter_p4_B.Gain1[4];

  /* Sum: '<Root>/Sum2' incorporates:
   *  Constant: '<Root>/Vs_bias'
   *  Gain: '<S3>/K_ed'
   *  Gain: '<S3>/K_ep'
   *  Sum: '<S3>/Sum1'
   */
  helicopter_p4_B.Sum2 = ((helicopter_p4_P.K_ep * rtb_Derivative + rtb_Backgain)
    - helicopter_p4_P.K_ed * helicopter_p4_B.Gain1[5]) + helicopter_p4_P.Vs_ff;
  if (rtmIsMajorTimeStep(helicopter_p4_M)) {
  }

  /* Gain: '<S1>/Back gain' incorporates:
   *  Sum: '<S1>/Subtract'
   */
  rtb_Backgain = (helicopter_p4_B.Sum2 - helicopter_p4_B.Sum1) *
    helicopter_p4_P.Backgain_Gain;

  /* Gain: '<S3>/K_ei' */
  helicopter_p4_B.K_ei = helicopter_p4_P.K_ei * rtb_Derivative;
  if (rtmIsMajorTimeStep(helicopter_p4_M)) {
  }

  /* Derivative: '<S4>/Derivative' */
  if ((helicopter_p4_DW.TimeStampA >= helicopter_p4_M->Timing.t[0]) &&
      (helicopter_p4_DW.TimeStampB >= helicopter_p4_M->Timing.t[0])) {
    rtb_Derivative = 0.0;
  } else {
    rtb_Derivative = helicopter_p4_DW.TimeStampA;
    lastU = &helicopter_p4_DW.LastUAtTimeA;
    if (helicopter_p4_DW.TimeStampA < helicopter_p4_DW.TimeStampB) {
      if (helicopter_p4_DW.TimeStampB < helicopter_p4_M->Timing.t[0]) {
        rtb_Derivative = helicopter_p4_DW.TimeStampB;
        lastU = &helicopter_p4_DW.LastUAtTimeB;
      }
    } else {
      if (helicopter_p4_DW.TimeStampA >= helicopter_p4_M->Timing.t[0]) {
        rtb_Derivative = helicopter_p4_DW.TimeStampB;
        lastU = &helicopter_p4_DW.LastUAtTimeB;
      }
    }

    rtb_Derivative = (helicopter_p4_B.PitchCounttorad - *lastU) /
      (helicopter_p4_M->Timing.t[0] - rtb_Derivative);
  }

  /* End of Derivative: '<S4>/Derivative' */

  /* Gain: '<S12>/Gain' */
  helicopter_p4_B.Gain_l = helicopter_p4_P.Gain_Gain_a1 * rtb_Derivative;
  if (rtmIsMajorTimeStep(helicopter_p4_M)) {
  }

  /* Saturate: '<S4>/Back motor: Saturation' */
  if (rtb_Backgain > helicopter_p4_P.BackmotorSaturation_UpperSat) {
    helicopter_p4_B.BackmotorSaturation =
      helicopter_p4_P.BackmotorSaturation_UpperSat;
  } else if (rtb_Backgain < helicopter_p4_P.BackmotorSaturation_LowerSat) {
    helicopter_p4_B.BackmotorSaturation =
      helicopter_p4_P.BackmotorSaturation_LowerSat;
  } else {
    helicopter_p4_B.BackmotorSaturation = rtb_Backgain;
  }

  /* End of Saturate: '<S4>/Back motor: Saturation' */
  if (rtmIsMajorTimeStep(helicopter_p4_M)) {
  }

  /* Gain: '<S1>/Front gain' incorporates:
   *  Sum: '<S1>/Add'
   */
  rtb_Derivative = (helicopter_p4_B.Sum1 + helicopter_p4_B.Sum2) *
    helicopter_p4_P.Frontgain_Gain;

  /* Saturate: '<S4>/Front motor: Saturation' */
  if (rtb_Derivative > helicopter_p4_P.FrontmotorSaturation_UpperSat) {
    helicopter_p4_B.FrontmotorSaturation =
      helicopter_p4_P.FrontmotorSaturation_UpperSat;
  } else if (rtb_Derivative < helicopter_p4_P.FrontmotorSaturation_LowerSat) {
    helicopter_p4_B.FrontmotorSaturation =
      helicopter_p4_P.FrontmotorSaturation_LowerSat;
  } else {
    helicopter_p4_B.FrontmotorSaturation = rtb_Derivative;
  }

  /* End of Saturate: '<S4>/Front motor: Saturation' */
  if (rtmIsMajorTimeStep(helicopter_p4_M)) {
    /* S-Function (hil_write_analog_block): '<S4>/HIL Write Analog' */

    /* S-Function Block: helicopter_p4/Helicopter_interface/HIL Write Analog (hil_write_analog_block) */
    {
      t_error result;
      helicopter_p4_DW.HILWriteAnalog_Buffer[0] =
        helicopter_p4_B.FrontmotorSaturation;
      helicopter_p4_DW.HILWriteAnalog_Buffer[1] =
        helicopter_p4_B.BackmotorSaturation;
      result = hil_write_analog(helicopter_p4_DW.HILInitialize_Card,
        helicopter_p4_P.HILWriteAnalog_channels, 2,
        &helicopter_p4_DW.HILWriteAnalog_Buffer[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p4_M, _rt_error_message);
      }
    }

    /* ToFile: '<S5>/ write to file ' */
    {
      if (!(++helicopter_p4_DW.writetofile_IWORK.Decimation % 1) &&
          (helicopter_p4_DW.writetofile_IWORK.Count*2)+1 < 100000000 ) {
        FILE *fp = (FILE *) helicopter_p4_DW.writetofile_PWORK.FilePtr;
        if (fp != (NULL)) {
          real_T u[2];
          helicopter_p4_DW.writetofile_IWORK.Decimation = 0;
          u[0] = helicopter_p4_M->Timing.t[1];
          u[1] = helicopter_p4_B.Sum5[0];
          if (fwrite(u, sizeof(real_T), 2, fp) != 2) {
            rtmSetErrorStatus(helicopter_p4_M,
                              "Error writing to MAT-file p_ref_practical.mat");
            return;
          }

          if (((++helicopter_p4_DW.writetofile_IWORK.Count)*2)+1 >= 100000000) {
            (void)fprintf(stdout,
                          "*** The ToFile block will stop logging data before\n"
                          "    the simulation has ended, because it has reached\n"
                          "    the maximum number of elements (100000000)\n"
                          "    allowed in MAT-file p_ref_practical.mat.\n");
          }
        }
      }
    }

    /* ToFile: '<S5>/write to file' */
    {
      if (!(++helicopter_p4_DW.writetofile_IWORK_g.Decimation % 1) &&
          (helicopter_p4_DW.writetofile_IWORK_g.Count*2)+1 < 100000000 ) {
        FILE *fp = (FILE *) helicopter_p4_DW.writetofile_PWORK_n.FilePtr;
        if (fp != (NULL)) {
          real_T u[2];
          helicopter_p4_DW.writetofile_IWORK_g.Decimation = 0;
          u[0] = helicopter_p4_M->Timing.t[1];
          u[1] = helicopter_p4_B.Sum5[1];
          if (fwrite(u, sizeof(real_T), 2, fp) != 2) {
            rtmSetErrorStatus(helicopter_p4_M,
                              "Error writing to MAT-file e_ref_practical.mat");
            return;
          }

          if (((++helicopter_p4_DW.writetofile_IWORK_g.Count)*2)+1 >= 100000000)
          {
            (void)fprintf(stdout,
                          "*** The ToFile block will stop logging data before\n"
                          "    the simulation has ended, because it has reached\n"
                          "    the maximum number of elements (100000000)\n"
                          "    allowed in MAT-file e_ref_practical.mat.\n");
          }
        }
      }
    }
  }
}

/* Model update function */
void helicopter_p4_update(void)
{
  real_T *lastU;

  /* Update for Derivative: '<S4>/Derivative' */
  if (helicopter_p4_DW.TimeStampA == (rtInf)) {
    helicopter_p4_DW.TimeStampA = helicopter_p4_M->Timing.t[0];
    lastU = &helicopter_p4_DW.LastUAtTimeA;
  } else if (helicopter_p4_DW.TimeStampB == (rtInf)) {
    helicopter_p4_DW.TimeStampB = helicopter_p4_M->Timing.t[0];
    lastU = &helicopter_p4_DW.LastUAtTimeB;
  } else if (helicopter_p4_DW.TimeStampA < helicopter_p4_DW.TimeStampB) {
    helicopter_p4_DW.TimeStampA = helicopter_p4_M->Timing.t[0];
    lastU = &helicopter_p4_DW.LastUAtTimeA;
  } else {
    helicopter_p4_DW.TimeStampB = helicopter_p4_M->Timing.t[0];
    lastU = &helicopter_p4_DW.LastUAtTimeB;
  }

  *lastU = helicopter_p4_B.PitchCounttorad;

  /* End of Update for Derivative: '<S4>/Derivative' */
  if (rtmIsMajorTimeStep(helicopter_p4_M)) {
    rt_ertODEUpdateContinuousStates(&helicopter_p4_M->solverInfo);
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
  if (!(++helicopter_p4_M->Timing.clockTick0)) {
    ++helicopter_p4_M->Timing.clockTickH0;
  }

  helicopter_p4_M->Timing.t[0] = rtsiGetSolverStopTime
    (&helicopter_p4_M->solverInfo);

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
    if (!(++helicopter_p4_M->Timing.clockTick1)) {
      ++helicopter_p4_M->Timing.clockTickH1;
    }

    helicopter_p4_M->Timing.t[1] = helicopter_p4_M->Timing.clockTick1 *
      helicopter_p4_M->Timing.stepSize1 + helicopter_p4_M->Timing.clockTickH1 *
      helicopter_p4_M->Timing.stepSize1 * 4294967296.0;
  }
}

/* Derivatives for root system: '<Root>' */
void helicopter_p4_derivatives(void)
{
  XDot_helicopter_p4_T *_rtXdot;
  _rtXdot = ((XDot_helicopter_p4_T *) helicopter_p4_M->ModelData.derivs);

  /* Derivatives for TransferFcn: '<S4>/Travel: Transfer Fcn' */
  _rtXdot->TravelTransferFcn_CSTATE = 0.0;
  _rtXdot->TravelTransferFcn_CSTATE += helicopter_p4_P.TravelTransferFcn_A *
    helicopter_p4_X.TravelTransferFcn_CSTATE;
  _rtXdot->TravelTransferFcn_CSTATE += helicopter_p4_B.Gain_g;

  /* Derivatives for TransferFcn: '<S4>/Pitch: Transfer Fcn' */
  _rtXdot->PitchTransferFcn_CSTATE = 0.0;
  _rtXdot->PitchTransferFcn_CSTATE += helicopter_p4_P.PitchTransferFcn_A *
    helicopter_p4_X.PitchTransferFcn_CSTATE;
  _rtXdot->PitchTransferFcn_CSTATE += helicopter_p4_B.PitchCounttorad;

  /* Derivatives for TransferFcn: '<S4>/Elevation: Transfer Fcn' */
  _rtXdot->ElevationTransferFcn_CSTATE = 0.0;
  _rtXdot->ElevationTransferFcn_CSTATE += helicopter_p4_P.ElevationTransferFcn_A
    * helicopter_p4_X.ElevationTransferFcn_CSTATE;
  _rtXdot->ElevationTransferFcn_CSTATE += helicopter_p4_B.ElevationCounttorad;

  /* Derivatives for Integrator: '<S3>/Integrator' */
  {
    boolean_T lsat;
    boolean_T usat;
    lsat = ( helicopter_p4_X.Integrator_CSTATE <=
            (helicopter_p4_P.Integrator_LowerSat) );
    usat = ( helicopter_p4_X.Integrator_CSTATE >=
            helicopter_p4_P.Integrator_UpperSat );
    if ((!lsat && !usat) ||
        (lsat && (helicopter_p4_B.K_ei > 0)) ||
        (usat && (helicopter_p4_B.K_ei < 0)) ) {
      ((XDot_helicopter_p4_T *) helicopter_p4_M->ModelData.derivs)
        ->Integrator_CSTATE = helicopter_p4_B.K_ei;
    } else {
      /* in saturation */
      ((XDot_helicopter_p4_T *) helicopter_p4_M->ModelData.derivs)
        ->Integrator_CSTATE = 0.0;
    }
  }
}

/* Model initialize function */
void helicopter_p4_initialize(void)
{
  /* Start for S-Function (hil_initialize_block): '<Root>/HIL Initialize' */

  /* S-Function Block: helicopter_p4/HIL Initialize (hil_initialize_block) */
  {
    t_int result;
    t_boolean is_switching;
    result = hil_open("q8_usb", "0", &helicopter_p4_DW.HILInitialize_Card);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(helicopter_p4_M, _rt_error_message);
      return;
    }

    is_switching = false;
    result = hil_set_card_specific_options(helicopter_p4_DW.HILInitialize_Card,
      "update_rate=normal;decimation=1", 32);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(helicopter_p4_M, _rt_error_message);
      return;
    }

    result = hil_watchdog_clear(helicopter_p4_DW.HILInitialize_Card);
    if (result < 0 && result != -QERR_HIL_WATCHDOG_CLEAR) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(helicopter_p4_M, _rt_error_message);
      return;
    }

    if ((helicopter_p4_P.HILInitialize_set_analog_input_ && !is_switching) ||
        (helicopter_p4_P.HILInitialize_set_analog_inpu_m && is_switching)) {
      {
        int_T i1;
        real_T *dw_AIMinimums = &helicopter_p4_DW.HILInitialize_AIMinimums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AIMinimums[i1] = helicopter_p4_P.HILInitialize_analog_input_mini;
        }
      }

      {
        int_T i1;
        real_T *dw_AIMaximums = &helicopter_p4_DW.HILInitialize_AIMaximums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AIMaximums[i1] = helicopter_p4_P.HILInitialize_analog_input_maxi;
        }
      }

      result = hil_set_analog_input_ranges(helicopter_p4_DW.HILInitialize_Card,
        helicopter_p4_P.HILInitialize_analog_input_chan, 8U,
        &helicopter_p4_DW.HILInitialize_AIMinimums[0],
        &helicopter_p4_DW.HILInitialize_AIMaximums[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p4_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_p4_P.HILInitialize_set_analog_output && !is_switching) ||
        (helicopter_p4_P.HILInitialize_set_analog_outp_b && is_switching)) {
      {
        int_T i1;
        real_T *dw_AOMinimums = &helicopter_p4_DW.HILInitialize_AOMinimums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOMinimums[i1] = helicopter_p4_P.HILInitialize_analog_output_min;
        }
      }

      {
        int_T i1;
        real_T *dw_AOMaximums = &helicopter_p4_DW.HILInitialize_AOMaximums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOMaximums[i1] = helicopter_p4_P.HILInitialize_analog_output_max;
        }
      }

      result = hil_set_analog_output_ranges(helicopter_p4_DW.HILInitialize_Card,
        helicopter_p4_P.HILInitialize_analog_output_cha, 8U,
        &helicopter_p4_DW.HILInitialize_AOMinimums[0],
        &helicopter_p4_DW.HILInitialize_AOMaximums[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p4_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_p4_P.HILInitialize_set_analog_outp_e && !is_switching) ||
        (helicopter_p4_P.HILInitialize_set_analog_outp_j && is_switching)) {
      {
        int_T i1;
        real_T *dw_AOVoltages = &helicopter_p4_DW.HILInitialize_AOVoltages[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOVoltages[i1] = helicopter_p4_P.HILInitialize_initial_analog_ou;
        }
      }

      result = hil_write_analog(helicopter_p4_DW.HILInitialize_Card,
        helicopter_p4_P.HILInitialize_analog_output_cha, 8U,
        &helicopter_p4_DW.HILInitialize_AOVoltages[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p4_M, _rt_error_message);
        return;
      }
    }

    if (helicopter_p4_P.HILInitialize_set_analog_outp_p) {
      {
        int_T i1;
        real_T *dw_AOVoltages = &helicopter_p4_DW.HILInitialize_AOVoltages[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOVoltages[i1] = helicopter_p4_P.HILInitialize_watchdog_analog_o;
        }
      }

      result = hil_watchdog_set_analog_expiration_state
        (helicopter_p4_DW.HILInitialize_Card,
         helicopter_p4_P.HILInitialize_analog_output_cha, 8U,
         &helicopter_p4_DW.HILInitialize_AOVoltages[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p4_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_p4_P.HILInitialize_set_encoder_param && !is_switching) ||
        (helicopter_p4_P.HILInitialize_set_encoder_par_m && is_switching)) {
      {
        int_T i1;
        int32_T *dw_QuadratureModes =
          &helicopter_p4_DW.HILInitialize_QuadratureModes[0];
        for (i1=0; i1 < 8; i1++) {
          dw_QuadratureModes[i1] = helicopter_p4_P.HILInitialize_quadrature;
        }
      }

      result = hil_set_encoder_quadrature_mode
        (helicopter_p4_DW.HILInitialize_Card,
         helicopter_p4_P.HILInitialize_encoder_channels, 8U,
         (t_encoder_quadrature_mode *)
         &helicopter_p4_DW.HILInitialize_QuadratureModes[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p4_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_p4_P.HILInitialize_set_encoder_count && !is_switching) ||
        (helicopter_p4_P.HILInitialize_set_encoder_cou_k && is_switching)) {
      {
        int_T i1;
        int32_T *dw_InitialEICounts =
          &helicopter_p4_DW.HILInitialize_InitialEICounts[0];
        for (i1=0; i1 < 8; i1++) {
          dw_InitialEICounts[i1] =
            helicopter_p4_P.HILInitialize_initial_encoder_c;
        }
      }

      result = hil_set_encoder_counts(helicopter_p4_DW.HILInitialize_Card,
        helicopter_p4_P.HILInitialize_encoder_channels, 8U,
        &helicopter_p4_DW.HILInitialize_InitialEICounts[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p4_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_p4_P.HILInitialize_set_pwm_params_at && !is_switching) ||
        (helicopter_p4_P.HILInitialize_set_pwm_params__f && is_switching)) {
      uint32_T num_duty_cycle_modes = 0;
      uint32_T num_frequency_modes = 0;

      {
        int_T i1;
        int32_T *dw_POModeValues = &helicopter_p4_DW.HILInitialize_POModeValues
          [0];
        for (i1=0; i1 < 8; i1++) {
          dw_POModeValues[i1] = helicopter_p4_P.HILInitialize_pwm_modes;
        }
      }

      result = hil_set_pwm_mode(helicopter_p4_DW.HILInitialize_Card,
        helicopter_p4_P.HILInitialize_pwm_channels, 8U, (t_pwm_mode *)
        &helicopter_p4_DW.HILInitialize_POModeValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p4_M, _rt_error_message);
        return;
      }

      {
        int_T i1;
        const uint32_T *p_HILInitialize_pwm_channels =
          helicopter_p4_P.HILInitialize_pwm_channels;
        int32_T *dw_POModeValues = &helicopter_p4_DW.HILInitialize_POModeValues
          [0];
        for (i1=0; i1 < 8; i1++) {
          if (dw_POModeValues[i1] == PWM_DUTY_CYCLE_MODE || dw_POModeValues[i1] ==
              PWM_ONE_SHOT_MODE || dw_POModeValues[i1] == PWM_TIME_MODE) {
            helicopter_p4_DW.HILInitialize_POSortedChans[num_duty_cycle_modes] =
              p_HILInitialize_pwm_channels[i1];
            helicopter_p4_DW.HILInitialize_POSortedFreqs[num_duty_cycle_modes] =
              helicopter_p4_P.HILInitialize_pwm_frequency;
            num_duty_cycle_modes++;
          } else {
            helicopter_p4_DW.HILInitialize_POSortedChans[7U -
              num_frequency_modes] = p_HILInitialize_pwm_channels[i1];
            helicopter_p4_DW.HILInitialize_POSortedFreqs[7U -
              num_frequency_modes] = helicopter_p4_P.HILInitialize_pwm_frequency;
            num_frequency_modes++;
          }
        }
      }

      if (num_duty_cycle_modes > 0) {
        result = hil_set_pwm_frequency(helicopter_p4_DW.HILInitialize_Card,
          &helicopter_p4_DW.HILInitialize_POSortedChans[0], num_duty_cycle_modes,
          &helicopter_p4_DW.HILInitialize_POSortedFreqs[0]);
        if (result < 0) {
          msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
            (_rt_error_message));
          rtmSetErrorStatus(helicopter_p4_M, _rt_error_message);
          return;
        }
      }

      if (num_frequency_modes > 0) {
        result = hil_set_pwm_duty_cycle(helicopter_p4_DW.HILInitialize_Card,
          &helicopter_p4_DW.HILInitialize_POSortedChans[num_duty_cycle_modes],
          num_frequency_modes,
          &helicopter_p4_DW.HILInitialize_POSortedFreqs[num_duty_cycle_modes]);
        if (result < 0) {
          msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
            (_rt_error_message));
          rtmSetErrorStatus(helicopter_p4_M, _rt_error_message);
          return;
        }
      }

      {
        int_T i1;
        int32_T *dw_POModeValues = &helicopter_p4_DW.HILInitialize_POModeValues
          [0];
        for (i1=0; i1 < 8; i1++) {
          dw_POModeValues[i1] = helicopter_p4_P.HILInitialize_pwm_configuration;
        }
      }

      {
        int_T i1;
        int32_T *dw_POAlignValues =
          &helicopter_p4_DW.HILInitialize_POAlignValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POAlignValues[i1] = helicopter_p4_P.HILInitialize_pwm_alignment;
        }
      }

      {
        int_T i1;
        int32_T *dw_POPolarityVals =
          &helicopter_p4_DW.HILInitialize_POPolarityVals[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POPolarityVals[i1] = helicopter_p4_P.HILInitialize_pwm_polarity;
        }
      }

      result = hil_set_pwm_configuration(helicopter_p4_DW.HILInitialize_Card,
        helicopter_p4_P.HILInitialize_pwm_channels, 8U,
        (t_pwm_configuration *) &helicopter_p4_DW.HILInitialize_POModeValues[0],
        (t_pwm_alignment *) &helicopter_p4_DW.HILInitialize_POAlignValues[0],
        (t_pwm_polarity *) &helicopter_p4_DW.HILInitialize_POPolarityVals[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p4_M, _rt_error_message);
        return;
      }

      {
        int_T i1;
        real_T *dw_POSortedFreqs =
          &helicopter_p4_DW.HILInitialize_POSortedFreqs[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POSortedFreqs[i1] = helicopter_p4_P.HILInitialize_pwm_leading_deadb;
        }
      }

      {
        int_T i1;
        real_T *dw_POValues = &helicopter_p4_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = helicopter_p4_P.HILInitialize_pwm_trailing_dead;
        }
      }

      result = hil_set_pwm_deadband(helicopter_p4_DW.HILInitialize_Card,
        helicopter_p4_P.HILInitialize_pwm_channels, 8U,
        &helicopter_p4_DW.HILInitialize_POSortedFreqs[0],
        &helicopter_p4_DW.HILInitialize_POValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p4_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_p4_P.HILInitialize_set_pwm_outputs_a && !is_switching) ||
        (helicopter_p4_P.HILInitialize_set_pwm_outputs_g && is_switching)) {
      {
        int_T i1;
        real_T *dw_POValues = &helicopter_p4_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = helicopter_p4_P.HILInitialize_initial_pwm_outpu;
        }
      }

      result = hil_write_pwm(helicopter_p4_DW.HILInitialize_Card,
        helicopter_p4_P.HILInitialize_pwm_channels, 8U,
        &helicopter_p4_DW.HILInitialize_POValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p4_M, _rt_error_message);
        return;
      }
    }

    if (helicopter_p4_P.HILInitialize_set_pwm_outputs_o) {
      {
        int_T i1;
        real_T *dw_POValues = &helicopter_p4_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = helicopter_p4_P.HILInitialize_watchdog_pwm_outp;
        }
      }

      result = hil_watchdog_set_pwm_expiration_state
        (helicopter_p4_DW.HILInitialize_Card,
         helicopter_p4_P.HILInitialize_pwm_channels, 8U,
         &helicopter_p4_DW.HILInitialize_POValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p4_M, _rt_error_message);
        return;
      }
    }
  }

  /* Start for S-Function (hil_read_encoder_timebase_block): '<S4>/HIL Read Encoder Timebase' */

  /* S-Function Block: helicopter_p4/Helicopter_interface/HIL Read Encoder Timebase (hil_read_encoder_timebase_block) */
  {
    t_error result;
    result = hil_task_create_encoder_reader(helicopter_p4_DW.HILInitialize_Card,
      helicopter_p4_P.HILReadEncoderTimebase_samples_,
      helicopter_p4_P.HILReadEncoderTimebase_channels, 3,
      &helicopter_p4_DW.HILReadEncoderTimebase_Task);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(helicopter_p4_M, _rt_error_message);
    }
  }

  /* Start for ToFile: '<S7>/To File' */
  {
    char fileName[509] = "p_dot_practical.mat";
    FILE *fp = (NULL);
    if ((fp = fopen(fileName, "wb")) == (NULL)) {
      rtmSetErrorStatus(helicopter_p4_M,
                        "Error creating .mat file p_dot_practical.mat");
      return;
    }

    if (rt_WriteMat4FileHeader(fp,2,0,"p_dot")) {
      rtmSetErrorStatus(helicopter_p4_M,
                        "Error writing mat file header to file p_dot_practical.mat");
      return;
    }

    helicopter_p4_DW.ToFile_IWORK.Count = 0;
    helicopter_p4_DW.ToFile_IWORK.Decimation = -1;
    helicopter_p4_DW.ToFile_PWORK.FilePtr = fp;
  }

  /* Start for ToFile: '<S7>/To File1' */
  {
    char fileName[509] = "p_practical.mat";
    FILE *fp = (NULL);
    if ((fp = fopen(fileName, "wb")) == (NULL)) {
      rtmSetErrorStatus(helicopter_p4_M,
                        "Error creating .mat file p_practical.mat");
      return;
    }

    if (rt_WriteMat4FileHeader(fp,2,0,"p")) {
      rtmSetErrorStatus(helicopter_p4_M,
                        "Error writing mat file header to file p_practical.mat");
      return;
    }

    helicopter_p4_DW.ToFile1_IWORK.Count = 0;
    helicopter_p4_DW.ToFile1_IWORK.Decimation = -1;
    helicopter_p4_DW.ToFile1_PWORK.FilePtr = fp;
  }

  /* Start for ToFile: '<S7>/To File2' */
  {
    char fileName[509] = "lambda_practical.mat";
    FILE *fp = (NULL);
    if ((fp = fopen(fileName, "wb")) == (NULL)) {
      rtmSetErrorStatus(helicopter_p4_M,
                        "Error creating .mat file lambda_practical.mat");
      return;
    }

    if (rt_WriteMat4FileHeader(fp,2,0,"lambda")) {
      rtmSetErrorStatus(helicopter_p4_M,
                        "Error writing mat file header to file lambda_practical.mat");
      return;
    }

    helicopter_p4_DW.ToFile2_IWORK.Count = 0;
    helicopter_p4_DW.ToFile2_IWORK.Decimation = -1;
    helicopter_p4_DW.ToFile2_PWORK.FilePtr = fp;
  }

  /* Start for ToFile: '<S7>/To File3' */
  {
    char fileName[509] = "e_dot_practical.mat";
    FILE *fp = (NULL);
    if ((fp = fopen(fileName, "wb")) == (NULL)) {
      rtmSetErrorStatus(helicopter_p4_M,
                        "Error creating .mat file e_dot_practical.mat");
      return;
    }

    if (rt_WriteMat4FileHeader(fp,2,0,"e_dot")) {
      rtmSetErrorStatus(helicopter_p4_M,
                        "Error writing mat file header to file e_dot_practical.mat");
      return;
    }

    helicopter_p4_DW.ToFile3_IWORK.Count = 0;
    helicopter_p4_DW.ToFile3_IWORK.Decimation = -1;
    helicopter_p4_DW.ToFile3_PWORK.FilePtr = fp;
  }

  /* Start for ToFile: '<S7>/To File4' */
  {
    char fileName[509] = "r_practical.mat";
    FILE *fp = (NULL);
    if ((fp = fopen(fileName, "wb")) == (NULL)) {
      rtmSetErrorStatus(helicopter_p4_M,
                        "Error creating .mat file r_practical.mat");
      return;
    }

    if (rt_WriteMat4FileHeader(fp,2,0,"r")) {
      rtmSetErrorStatus(helicopter_p4_M,
                        "Error writing mat file header to file r_practical.mat");
      return;
    }

    helicopter_p4_DW.ToFile4_IWORK.Count = 0;
    helicopter_p4_DW.ToFile4_IWORK.Decimation = -1;
    helicopter_p4_DW.ToFile4_PWORK.FilePtr = fp;
  }

  /* Start for ToFile: '<S7>/To File5' */
  {
    char fileName[509] = "e_practical.mat";
    FILE *fp = (NULL);
    if ((fp = fopen(fileName, "wb")) == (NULL)) {
      rtmSetErrorStatus(helicopter_p4_M,
                        "Error creating .mat file e_practical.mat");
      return;
    }

    if (rt_WriteMat4FileHeader(fp,2,0,"e")) {
      rtmSetErrorStatus(helicopter_p4_M,
                        "Error writing mat file header to file e_practical.mat");
      return;
    }

    helicopter_p4_DW.ToFile5_IWORK.Count = 0;
    helicopter_p4_DW.ToFile5_IWORK.Decimation = -1;
    helicopter_p4_DW.ToFile5_PWORK.FilePtr = fp;
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
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      -0.52359877559829882, -0.52359877559829882, -0.52359877559829882,
      -0.52359877559829882, -0.52359877559829882, -0.52359877559829882,
      -0.52359877559829882, -0.52359877559829882, -0.52359877559829882,
      -0.52297778128506889, -0.30514598607582566, -0.12345046052756414,
      0.025305923543566981, 0.14422541725092405, 0.23587358457380792,
      0.30339639827489956, 0.34985064574768976, 0.37815964601849067,
      0.39108543315630179, 0.39119904305357034, 0.38087423491311506,
      0.36227238518880378, 0.33734721381686417, 0.30784727414617391,
      0.27532858430684209, 0.24116187230056746, 0.20655468155694556,
      0.17255638949334928, 0.14008142428852569, 0.10991229805337877,
      0.082713621921240682, 0.059028291249546262, 0.039270488843235259,
      0.023695226329119242, 0.012361458219653156, 0.0050641727769098364,
      0.0012577364331938974, 6.0401972568996036E-7, 3.5388258378815774E-7,
      -3.7290872083678145E-7, -3.7290872083678145E-7, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.10358041217061946, 0.12023066339867879,
      0.13873035370598213, 0.15895777761251578, 0.18063275354497044,
      0.20320952979440035, 0.2257899116604748, 0.24695675569948888,
      0.2645979419461969, 0.2756229069070083, 0.27562446948741781,
      0.25839787183585966, 0.21533098252159069, 0.13458453475181745,
      1.7090848565522862E-6, 2.8548222823809276E-6, 3.0220689327880555E-6,
      -1.3731245170925861E-6, -1.3229364221187462E-6, 2.2379315113418755E-6,
      2.2381953584028234E-6, 5.10232465335738E-7, -3.1320867289209469E-6,
      -4.01098026827962E-7, -3.3455928953984802E-6, -3.6144630089978645E-6,
      -3.3321664429909604E-6, -1.5446588727986774E-6, -3.1878030488453006E-6,
      -4.2449268583284722E-6, -3.4498214434930735E-6, -3.6249467207018031E-6,
      -2.7898377208580681E-6, -6.1388479005148208E-7, -2.3312318456907706E-6,
      -1.0521132331296273E-6, 8.1034592598799229E-7, 1.4111229540268973E-6,
      4.2941772458246384E-7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 } ;

    helicopter_p4_DW._PWORK.TimePtr = (void *) pTimeValues0;
    helicopter_p4_DW._PWORK.DataPtr = (void *) pDataValues0;
    helicopter_p4_DW._IWORK.PrevIndex = 0;
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
      2.633505110490284, 2.49320005418376, 2.3360971319007047,
      2.1651555075747786, 1.984253930542891, 1.7976156924589204,
      1.6093695637739358, 1.4232633088459097, 1.2425032648925993,
      1.0696854801091555, 0.90678827727817324, 0.75520332066356943,
      0.61578904222200181, 0.48893549453306206, 0.37463352199315814,
      0.27254375591887431, 0.18206273055570418, 0.10238455812601926,
      0.032557420863272173, -0.028465427813661504, -0.081780100845071341,
      -0.1284917815890084, -0.16967942035170636, -0.20636520136062739,
      -0.23948866896333895, -0.2698856760512155, -0.298272631862083,
      -0.32523701115393366, -0.35123558768206981, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.015002048909068423,
      -0.046506351618112111, -0.091625013712135384, -0.14672753935514371,
      -0.20869397118807925, -0.27519058110636674, -0.34459405529608839,
      -0.41582470131356869, -0.48818590989947469, -0.56122022522609671,
      -0.62841168913222056, -0.68376649730370442, -0.72360630812755078,
      -0.74655295233588292, -0.75298451473993844, -0.7444250197121044,
      -0.72304017581324176, -0.69127113913377514, -0.65158881132392876,
      -0.60633982645841555, -0.55765711376627025, -0.507414190755759,
      -0.45720789015961566, -0.40835906429713531, -0.36192410145268039,
      -0.31871268971873967, -0.2793085490509884, -0.24409139470773469,
      -0.21325869212563936, -0.18684672297574823, -0.16475055505079184,
      -0.14674312403568418, -0.13249387041084623, -0.12158802835150628,
      -0.1135478232434699, -0.10785751716740255, -0.10399430611254482,
      -0.10146607423395755, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, -0.10602875205865551, -0.22266037932317656, -0.31888147181640641,
      -0.38944360631144165, -0.43795507377677839, -0.46997264230390068,
      -0.49051724877547076, -0.5034310014147434, -0.5114213858602934,
      -0.5161786472285893, -0.47488360490906412, -0.39122664287015407,
      -0.28157256715460527, -0.16217811740853202, -0.045455826708921038,
      0.060495241786934967, 0.15113990930898424, 0.22453141791807923,
      0.28045953735807055, 0.31980254339200664, 0.34407082025029678,
      0.35509779089997723, 0.35483895766217516, 0.34524484469574979,
      0.32818458279465229, 0.30540175469451813, 0.27849341688386908,
      0.24890139664956751, 0.2179137661255803, 0.18666971060745335,
      0.15616727585426191, 0.12726964495896656, 0.10070828249970414,
      0.0770783264812338, 0.05682509896267645, 0.040216910085695112,
      0.02730370028577676, 0.017868577327426114, 0.011396137384944473, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.42411500823462206,
      -0.46652650905808424, -0.38488436997291947, -0.282248537980141,
      -0.19404586986134689, -0.12807027410848895, -0.08217842588628041,
      -0.051655010557090569, -0.031961537782199782, -0.019029045473183591,
      0.16518016927810059, 0.33462784815564039, 0.43861630286219527,
      0.47757779898429281, 0.46688916279844395, 0.423804273983424,
      0.36257867008819716, 0.29356603443637985, 0.22371247775996517,
      0.15737202413574436, 0.097073107433160472, 0.044107882598721684,
      -0.0010353329512082746, -0.038376451865701496, -0.06824104760438994,
      -0.091131312400536651, -0.10763335124259611, -0.1183680809372064,
      -0.12395052209594883, -0.12497622207250776, -0.12200973901276588,
      -0.11559052358118137, -0.10624544983704973, -0.0945198240738813,
      -0.081012910074229422, -0.066432755507925365, -0.051652839199673409,
      -0.03774049183340257, -0.025889759769926565, -0.017062825668271688, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0041432164868247792, 0.011438372914866797, 0.021198952260458304,
      0.032956076055719166, 0.046387702384256171, 0.061256816330925626,
      0.077354373069976043, 0.094440904688148822, 0.11218256641410133,
      0.13007484353842722, 0.14734788593595541, 0.16284463250636963,
      0.17486200431204357, 0.18094202348526586, 0.17759562318011174,
      0.16835021625049992, 0.15569926804828566, 0.14137463555195645,
      0.12655183241477058, 0.11200325462764127, 0.0982121241865872,
      0.085457336146147714, 0.073875853070951375, 0.0635086537360666,
      0.054333166188581847, 0.046287382932127981, 0.039286453044054669,
      0.033234338007570648, 0.028031483351796095, 0.023580227240954207,
      0.019788076247519486, 0.016569431563951659, 0.013846610110401348,
      0.011550115420321494, 0.0096182609525837012, 0.0079971015705988385,
      0.0066397079171416126, 0.0055054441071614854, 0.0045593146811967267, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.016572865947299117,
      0.029180625712168076, 0.039042317382366028, 0.047028495181043455,
      0.053726505314148021, 0.059476455786677804, 0.064390226956201668,
      0.068346126472691129, 0.070966646903810021, 0.071569108497303524,
      0.069092169590112759, 0.061986986281656851, 0.048069487222695752,
      0.024320076692889103, -0.013385601220616467, -0.036981627718447242,
      -0.050603792808856983, -0.057298529985316919, -0.059291212548743394,
      -0.05819431114851726, -0.0551645217642163, -0.051019152161757939,
      -0.046325932300785334, -0.041468797339539128, -0.036701950189938964,
      -0.03218313302581547, -0.028003719552293256, -0.02420846014593608,
      -0.02081141862309821, -0.017805024443367564, -0.015168603973738872,
      -0.01287457873427131, -0.010891285814201242, -0.00918597876031942,
      -0.0077274178709511778, -0.0064846375279394471, -0.0054295746138289021,
      -0.0045370552399205113, -0.0037845177038590321, -0.0031515816794612569,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0 } ;

    helicopter_p4_DW._PWORK_k.TimePtr = (void *) pTimeValues0;
    helicopter_p4_DW._PWORK_k.DataPtr = (void *) pDataValues0;
    helicopter_p4_DW._IWORK_g.PrevIndex = 0;
  }

  /* Start for ToFile: '<S5>/ write to file ' */
  {
    char fileName[509] = "p_ref_practical.mat";
    FILE *fp = (NULL);
    if ((fp = fopen(fileName, "wb")) == (NULL)) {
      rtmSetErrorStatus(helicopter_p4_M,
                        "Error creating .mat file p_ref_practical.mat");
      return;
    }

    if (rt_WriteMat4FileHeader(fp,2,0,"p_ref")) {
      rtmSetErrorStatus(helicopter_p4_M,
                        "Error writing mat file header to file p_ref_practical.mat");
      return;
    }

    helicopter_p4_DW.writetofile_IWORK.Count = 0;
    helicopter_p4_DW.writetofile_IWORK.Decimation = -1;
    helicopter_p4_DW.writetofile_PWORK.FilePtr = fp;
  }

  /* Start for ToFile: '<S5>/write to file' */
  {
    char fileName[509] = "e_ref_practical.mat";
    FILE *fp = (NULL);
    if ((fp = fopen(fileName, "wb")) == (NULL)) {
      rtmSetErrorStatus(helicopter_p4_M,
                        "Error creating .mat file e_ref_practical.mat");
      return;
    }

    if (rt_WriteMat4FileHeader(fp,2,0,"e_ref")) {
      rtmSetErrorStatus(helicopter_p4_M,
                        "Error writing mat file header to file e_ref_practical.mat");
      return;
    }

    helicopter_p4_DW.writetofile_IWORK_g.Count = 0;
    helicopter_p4_DW.writetofile_IWORK_g.Decimation = -1;
    helicopter_p4_DW.writetofile_PWORK_n.FilePtr = fp;
  }

  /* InitializeConditions for TransferFcn: '<S4>/Travel: Transfer Fcn' */
  helicopter_p4_X.TravelTransferFcn_CSTATE = 0.0;

  /* InitializeConditions for TransferFcn: '<S4>/Pitch: Transfer Fcn' */
  helicopter_p4_X.PitchTransferFcn_CSTATE = 0.0;

  /* InitializeConditions for TransferFcn: '<S4>/Elevation: Transfer Fcn' */
  helicopter_p4_X.ElevationTransferFcn_CSTATE = 0.0;

  /* InitializeConditions for Integrator: '<S3>/Integrator' */
  helicopter_p4_X.Integrator_CSTATE = helicopter_p4_P.Integrator_IC;

  /* InitializeConditions for Derivative: '<S4>/Derivative' */
  helicopter_p4_DW.TimeStampA = (rtInf);
  helicopter_p4_DW.TimeStampB = (rtInf);
}

/* Model terminate function */
void helicopter_p4_terminate(void)
{
  /* Terminate for S-Function (hil_initialize_block): '<Root>/HIL Initialize' */

  /* S-Function Block: helicopter_p4/HIL Initialize (hil_initialize_block) */
  {
    t_boolean is_switching;
    t_int result;
    t_uint32 num_final_analog_outputs = 0;
    t_uint32 num_final_pwm_outputs = 0;
    hil_task_stop_all(helicopter_p4_DW.HILInitialize_Card);
    hil_monitor_stop_all(helicopter_p4_DW.HILInitialize_Card);
    is_switching = false;
    if ((helicopter_p4_P.HILInitialize_set_analog_out_ex && !is_switching) ||
        (helicopter_p4_P.HILInitialize_set_analog_outp_c && is_switching)) {
      {
        int_T i1;
        real_T *dw_AOVoltages = &helicopter_p4_DW.HILInitialize_AOVoltages[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOVoltages[i1] = helicopter_p4_P.HILInitialize_final_analog_outp;
        }
      }

      num_final_analog_outputs = 8U;
    }

    if ((helicopter_p4_P.HILInitialize_set_pwm_output_ap && !is_switching) ||
        (helicopter_p4_P.HILInitialize_set_pwm_outputs_p && is_switching)) {
      {
        int_T i1;
        real_T *dw_POValues = &helicopter_p4_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = helicopter_p4_P.HILInitialize_final_pwm_outputs;
        }
      }

      num_final_pwm_outputs = 8U;
    }

    if (0
        || num_final_analog_outputs > 0
        || num_final_pwm_outputs > 0
        ) {
      /* Attempt to write the final outputs atomically (due to firmware issue in old Q2-USB). Otherwise write channels individually */
      result = hil_write(helicopter_p4_DW.HILInitialize_Card
                         , helicopter_p4_P.HILInitialize_analog_output_cha,
                         num_final_analog_outputs
                         , helicopter_p4_P.HILInitialize_pwm_channels,
                         num_final_pwm_outputs
                         , NULL, 0
                         , NULL, 0
                         , &helicopter_p4_DW.HILInitialize_AOVoltages[0]
                         , &helicopter_p4_DW.HILInitialize_POValues[0]
                         , (t_boolean *) NULL
                         , NULL
                         );
      if (result == -QERR_HIL_WRITE_NOT_SUPPORTED) {
        t_error local_result;
        result = 0;

        /* The hil_write operation is not supported by this card. Write final outputs for each channel type */
        if (num_final_analog_outputs > 0) {
          local_result = hil_write_analog(helicopter_p4_DW.HILInitialize_Card,
            helicopter_p4_P.HILInitialize_analog_output_cha,
            num_final_analog_outputs,
            &helicopter_p4_DW.HILInitialize_AOVoltages[0]);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (num_final_pwm_outputs > 0) {
          local_result = hil_write_pwm(helicopter_p4_DW.HILInitialize_Card,
            helicopter_p4_P.HILInitialize_pwm_channels, num_final_pwm_outputs,
            &helicopter_p4_DW.HILInitialize_POValues[0]);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (result < 0) {
          msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
            (_rt_error_message));
          rtmSetErrorStatus(helicopter_p4_M, _rt_error_message);
        }
      }
    }

    hil_task_delete_all(helicopter_p4_DW.HILInitialize_Card);
    hil_monitor_delete_all(helicopter_p4_DW.HILInitialize_Card);
    hil_close(helicopter_p4_DW.HILInitialize_Card);
    helicopter_p4_DW.HILInitialize_Card = NULL;
  }

  /* Terminate for ToFile: '<S7>/To File' */
  {
    FILE *fp = (FILE *) helicopter_p4_DW.ToFile_PWORK.FilePtr;
    if (fp != (NULL)) {
      char fileName[509] = "p_dot_practical.mat";
      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_p4_M,
                          "Error closing MAT-file p_dot_practical.mat");
        return;
      }

      if ((fp = fopen(fileName, "r+b")) == (NULL)) {
        rtmSetErrorStatus(helicopter_p4_M,
                          "Error reopening MAT-file p_dot_practical.mat");
        return;
      }

      if (rt_WriteMat4FileHeader(fp, 2, helicopter_p4_DW.ToFile_IWORK.Count,
           "p_dot")) {
        rtmSetErrorStatus(helicopter_p4_M,
                          "Error writing header for p_dot to MAT-file p_dot_practical.mat");
      }

      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_p4_M,
                          "Error closing MAT-file p_dot_practical.mat");
        return;
      }

      helicopter_p4_DW.ToFile_PWORK.FilePtr = (NULL);
    }
  }

  /* Terminate for ToFile: '<S7>/To File1' */
  {
    FILE *fp = (FILE *) helicopter_p4_DW.ToFile1_PWORK.FilePtr;
    if (fp != (NULL)) {
      char fileName[509] = "p_practical.mat";
      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_p4_M,
                          "Error closing MAT-file p_practical.mat");
        return;
      }

      if ((fp = fopen(fileName, "r+b")) == (NULL)) {
        rtmSetErrorStatus(helicopter_p4_M,
                          "Error reopening MAT-file p_practical.mat");
        return;
      }

      if (rt_WriteMat4FileHeader(fp, 2, helicopter_p4_DW.ToFile1_IWORK.Count,
           "p")) {
        rtmSetErrorStatus(helicopter_p4_M,
                          "Error writing header for p to MAT-file p_practical.mat");
      }

      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_p4_M,
                          "Error closing MAT-file p_practical.mat");
        return;
      }

      helicopter_p4_DW.ToFile1_PWORK.FilePtr = (NULL);
    }
  }

  /* Terminate for ToFile: '<S7>/To File2' */
  {
    FILE *fp = (FILE *) helicopter_p4_DW.ToFile2_PWORK.FilePtr;
    if (fp != (NULL)) {
      char fileName[509] = "lambda_practical.mat";
      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_p4_M,
                          "Error closing MAT-file lambda_practical.mat");
        return;
      }

      if ((fp = fopen(fileName, "r+b")) == (NULL)) {
        rtmSetErrorStatus(helicopter_p4_M,
                          "Error reopening MAT-file lambda_practical.mat");
        return;
      }

      if (rt_WriteMat4FileHeader(fp, 2, helicopter_p4_DW.ToFile2_IWORK.Count,
           "lambda")) {
        rtmSetErrorStatus(helicopter_p4_M,
                          "Error writing header for lambda to MAT-file lambda_practical.mat");
      }

      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_p4_M,
                          "Error closing MAT-file lambda_practical.mat");
        return;
      }

      helicopter_p4_DW.ToFile2_PWORK.FilePtr = (NULL);
    }
  }

  /* Terminate for ToFile: '<S7>/To File3' */
  {
    FILE *fp = (FILE *) helicopter_p4_DW.ToFile3_PWORK.FilePtr;
    if (fp != (NULL)) {
      char fileName[509] = "e_dot_practical.mat";
      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_p4_M,
                          "Error closing MAT-file e_dot_practical.mat");
        return;
      }

      if ((fp = fopen(fileName, "r+b")) == (NULL)) {
        rtmSetErrorStatus(helicopter_p4_M,
                          "Error reopening MAT-file e_dot_practical.mat");
        return;
      }

      if (rt_WriteMat4FileHeader(fp, 2, helicopter_p4_DW.ToFile3_IWORK.Count,
           "e_dot")) {
        rtmSetErrorStatus(helicopter_p4_M,
                          "Error writing header for e_dot to MAT-file e_dot_practical.mat");
      }

      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_p4_M,
                          "Error closing MAT-file e_dot_practical.mat");
        return;
      }

      helicopter_p4_DW.ToFile3_PWORK.FilePtr = (NULL);
    }
  }

  /* Terminate for ToFile: '<S7>/To File4' */
  {
    FILE *fp = (FILE *) helicopter_p4_DW.ToFile4_PWORK.FilePtr;
    if (fp != (NULL)) {
      char fileName[509] = "r_practical.mat";
      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_p4_M,
                          "Error closing MAT-file r_practical.mat");
        return;
      }

      if ((fp = fopen(fileName, "r+b")) == (NULL)) {
        rtmSetErrorStatus(helicopter_p4_M,
                          "Error reopening MAT-file r_practical.mat");
        return;
      }

      if (rt_WriteMat4FileHeader(fp, 2, helicopter_p4_DW.ToFile4_IWORK.Count,
           "r")) {
        rtmSetErrorStatus(helicopter_p4_M,
                          "Error writing header for r to MAT-file r_practical.mat");
      }

      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_p4_M,
                          "Error closing MAT-file r_practical.mat");
        return;
      }

      helicopter_p4_DW.ToFile4_PWORK.FilePtr = (NULL);
    }
  }

  /* Terminate for ToFile: '<S7>/To File5' */
  {
    FILE *fp = (FILE *) helicopter_p4_DW.ToFile5_PWORK.FilePtr;
    if (fp != (NULL)) {
      char fileName[509] = "e_practical.mat";
      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_p4_M,
                          "Error closing MAT-file e_practical.mat");
        return;
      }

      if ((fp = fopen(fileName, "r+b")) == (NULL)) {
        rtmSetErrorStatus(helicopter_p4_M,
                          "Error reopening MAT-file e_practical.mat");
        return;
      }

      if (rt_WriteMat4FileHeader(fp, 2, helicopter_p4_DW.ToFile5_IWORK.Count,
           "e")) {
        rtmSetErrorStatus(helicopter_p4_M,
                          "Error writing header for e to MAT-file e_practical.mat");
      }

      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_p4_M,
                          "Error closing MAT-file e_practical.mat");
        return;
      }

      helicopter_p4_DW.ToFile5_PWORK.FilePtr = (NULL);
    }
  }

  /* Terminate for ToFile: '<S5>/ write to file ' */
  {
    FILE *fp = (FILE *) helicopter_p4_DW.writetofile_PWORK.FilePtr;
    if (fp != (NULL)) {
      char fileName[509] = "p_ref_practical.mat";
      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_p4_M,
                          "Error closing MAT-file p_ref_practical.mat");
        return;
      }

      if ((fp = fopen(fileName, "r+b")) == (NULL)) {
        rtmSetErrorStatus(helicopter_p4_M,
                          "Error reopening MAT-file p_ref_practical.mat");
        return;
      }

      if (rt_WriteMat4FileHeader(fp, 2, helicopter_p4_DW.writetofile_IWORK.Count,
           "p_ref")) {
        rtmSetErrorStatus(helicopter_p4_M,
                          "Error writing header for p_ref to MAT-file p_ref_practical.mat");
      }

      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_p4_M,
                          "Error closing MAT-file p_ref_practical.mat");
        return;
      }

      helicopter_p4_DW.writetofile_PWORK.FilePtr = (NULL);
    }
  }

  /* Terminate for ToFile: '<S5>/write to file' */
  {
    FILE *fp = (FILE *) helicopter_p4_DW.writetofile_PWORK_n.FilePtr;
    if (fp != (NULL)) {
      char fileName[509] = "e_ref_practical.mat";
      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_p4_M,
                          "Error closing MAT-file e_ref_practical.mat");
        return;
      }

      if ((fp = fopen(fileName, "r+b")) == (NULL)) {
        rtmSetErrorStatus(helicopter_p4_M,
                          "Error reopening MAT-file e_ref_practical.mat");
        return;
      }

      if (rt_WriteMat4FileHeader(fp, 2,
           helicopter_p4_DW.writetofile_IWORK_g.Count, "e_ref")) {
        rtmSetErrorStatus(helicopter_p4_M,
                          "Error writing header for e_ref to MAT-file e_ref_practical.mat");
      }

      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_p4_M,
                          "Error closing MAT-file e_ref_practical.mat");
        return;
      }

      helicopter_p4_DW.writetofile_PWORK_n.FilePtr = (NULL);
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
  helicopter_p4_output();
  UNUSED_PARAMETER(tid);
}

void MdlUpdate(int_T tid)
{
  helicopter_p4_update();
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
  helicopter_p4_initialize();
}

void MdlTerminate(void)
{
  helicopter_p4_terminate();
}

/* Registration function */
RT_MODEL_helicopter_p4_T *helicopter_p4(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* non-finite (run-time) assignments */
  helicopter_p4_P.Integrator_UpperSat = rtInf;
  helicopter_p4_P.Integrator_LowerSat = rtMinusInf;

  /* initialize real-time model */
  (void) memset((void *)helicopter_p4_M, 0,
                sizeof(RT_MODEL_helicopter_p4_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&helicopter_p4_M->solverInfo,
                          &helicopter_p4_M->Timing.simTimeStep);
    rtsiSetTPtr(&helicopter_p4_M->solverInfo, &rtmGetTPtr(helicopter_p4_M));
    rtsiSetStepSizePtr(&helicopter_p4_M->solverInfo,
                       &helicopter_p4_M->Timing.stepSize0);
    rtsiSetdXPtr(&helicopter_p4_M->solverInfo,
                 &helicopter_p4_M->ModelData.derivs);
    rtsiSetContStatesPtr(&helicopter_p4_M->solverInfo, (real_T **)
                         &helicopter_p4_M->ModelData.contStates);
    rtsiSetNumContStatesPtr(&helicopter_p4_M->solverInfo,
      &helicopter_p4_M->Sizes.numContStates);
    rtsiSetErrorStatusPtr(&helicopter_p4_M->solverInfo, (&rtmGetErrorStatus
      (helicopter_p4_M)));
    rtsiSetRTModelPtr(&helicopter_p4_M->solverInfo, helicopter_p4_M);
  }

  rtsiSetSimTimeStep(&helicopter_p4_M->solverInfo, MAJOR_TIME_STEP);
  helicopter_p4_M->ModelData.intgData.f[0] = helicopter_p4_M->ModelData.odeF[0];
  helicopter_p4_M->ModelData.contStates = ((real_T *) &helicopter_p4_X);
  rtsiSetSolverData(&helicopter_p4_M->solverInfo, (void *)
                    &helicopter_p4_M->ModelData.intgData);
  rtsiSetSolverName(&helicopter_p4_M->solverInfo,"ode1");

  /* Initialize timing info */
  {
    int_T *mdlTsMap = helicopter_p4_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    mdlTsMap[1] = 1;
    helicopter_p4_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    helicopter_p4_M->Timing.sampleTimes =
      (&helicopter_p4_M->Timing.sampleTimesArray[0]);
    helicopter_p4_M->Timing.offsetTimes =
      (&helicopter_p4_M->Timing.offsetTimesArray[0]);

    /* task periods */
    helicopter_p4_M->Timing.sampleTimes[0] = (0.0);
    helicopter_p4_M->Timing.sampleTimes[1] = (0.002);

    /* task offsets */
    helicopter_p4_M->Timing.offsetTimes[0] = (0.0);
    helicopter_p4_M->Timing.offsetTimes[1] = (0.0);
  }

  rtmSetTPtr(helicopter_p4_M, &helicopter_p4_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits = helicopter_p4_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    mdlSampleHits[1] = 1;
    helicopter_p4_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(helicopter_p4_M, 20.0);
  helicopter_p4_M->Timing.stepSize0 = 0.002;
  helicopter_p4_M->Timing.stepSize1 = 0.002;

  /* External mode info */
  helicopter_p4_M->Sizes.checksums[0] = (4204266953U);
  helicopter_p4_M->Sizes.checksums[1] = (197903978U);
  helicopter_p4_M->Sizes.checksums[2] = (1835051627U);
  helicopter_p4_M->Sizes.checksums[3] = (1837469240U);

  {
    static const sysRanDType rtAlwaysEnabled = SUBSYS_RAN_BC_ENABLE;
    static RTWExtModeInfo rt_ExtModeInfo;
    static const sysRanDType *systemRan[1];
    helicopter_p4_M->extModeInfo = (&rt_ExtModeInfo);
    rteiSetSubSystemActiveVectorAddresses(&rt_ExtModeInfo, systemRan);
    systemRan[0] = &rtAlwaysEnabled;
    rteiSetModelMappingInfoPtr(helicopter_p4_M->extModeInfo,
      &helicopter_p4_M->SpecialInfo.mappingInfo);
    rteiSetChecksumsPtr(helicopter_p4_M->extModeInfo,
                        helicopter_p4_M->Sizes.checksums);
    rteiSetTPtr(helicopter_p4_M->extModeInfo, rtmGetTPtr(helicopter_p4_M));
  }

  helicopter_p4_M->solverInfoPtr = (&helicopter_p4_M->solverInfo);
  helicopter_p4_M->Timing.stepSize = (0.002);
  rtsiSetFixedStepSize(&helicopter_p4_M->solverInfo, 0.002);
  rtsiSetSolverMode(&helicopter_p4_M->solverInfo, SOLVER_MODE_SINGLETASKING);

  /* block I/O */
  helicopter_p4_M->ModelData.blockIO = ((void *) &helicopter_p4_B);

  {
    int_T i;
    for (i = 0; i < 6; i++) {
      helicopter_p4_B.Gain1[i] = 0.0;
    }

    helicopter_p4_B.ElevationCounttorad = 0.0;
    helicopter_p4_B.Gain = 0.0;
    helicopter_p4_B.Sum = 0.0;
    helicopter_p4_B.PitchCounttorad = 0.0;
    helicopter_p4_B.Gain_i = 0.0;
    helicopter_p4_B.Gain_g = 0.0;
    helicopter_p4_B.Gain_p = 0.0;
    helicopter_p4_B.Sum3 = 0.0;
    helicopter_p4_B.Gain_d = 0.0;
    helicopter_p4_B.Gain_b = 0.0;
    helicopter_p4_B.Gain_dg = 0.0;
    helicopter_p4_B.Sum5[0] = 0.0;
    helicopter_p4_B.Sum5[1] = 0.0;
    helicopter_p4_B.Sum1 = 0.0;
    helicopter_p4_B.Sum2 = 0.0;
    helicopter_p4_B.K_ei = 0.0;
    helicopter_p4_B.Gain_l = 0.0;
    helicopter_p4_B.BackmotorSaturation = 0.0;
    helicopter_p4_B.FrontmotorSaturation = 0.0;
  }

  /* parameters */
  helicopter_p4_M->ModelData.defaultParam = ((real_T *)&helicopter_p4_P);

  /* states (continuous) */
  {
    real_T *x = (real_T *) &helicopter_p4_X;
    helicopter_p4_M->ModelData.contStates = (x);
    (void) memset((void *)&helicopter_p4_X, 0,
                  sizeof(X_helicopter_p4_T));
  }

  /* states (dwork) */
  helicopter_p4_M->ModelData.dwork = ((void *) &helicopter_p4_DW);
  (void) memset((void *)&helicopter_p4_DW, 0,
                sizeof(DW_helicopter_p4_T));

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      helicopter_p4_DW.HILInitialize_AIMinimums[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      helicopter_p4_DW.HILInitialize_AIMaximums[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      helicopter_p4_DW.HILInitialize_AOMinimums[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      helicopter_p4_DW.HILInitialize_AOMaximums[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      helicopter_p4_DW.HILInitialize_AOVoltages[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      helicopter_p4_DW.HILInitialize_FilterFrequency[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      helicopter_p4_DW.HILInitialize_POSortedFreqs[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      helicopter_p4_DW.HILInitialize_POValues[i] = 0.0;
    }
  }

  helicopter_p4_DW.TimeStampA = 0.0;
  helicopter_p4_DW.LastUAtTimeA = 0.0;
  helicopter_p4_DW.TimeStampB = 0.0;
  helicopter_p4_DW.LastUAtTimeB = 0.0;
  helicopter_p4_DW.HILWriteAnalog_Buffer[0] = 0.0;
  helicopter_p4_DW.HILWriteAnalog_Buffer[1] = 0.0;

  /* data type transition information */
  {
    static DataTypeTransInfo dtInfo;
    (void) memset((char_T *) &dtInfo, 0,
                  sizeof(dtInfo));
    helicopter_p4_M->SpecialInfo.mappingInfo = (&dtInfo);
    dtInfo.numDataTypes = 16;
    dtInfo.dataTypeSizes = &rtDataTypeSizes[0];
    dtInfo.dataTypeNames = &rtDataTypeNames[0];

    /* Block I/O transition table */
    dtInfo.B = &rtBTransTable;

    /* Parameters transition table */
    dtInfo.P = &rtPTransTable;
  }

  /* Initialize Sizes */
  helicopter_p4_M->Sizes.numContStates = (4);/* Number of continuous states */
  helicopter_p4_M->Sizes.numY = (0);   /* Number of model outputs */
  helicopter_p4_M->Sizes.numU = (0);   /* Number of model inputs */
  helicopter_p4_M->Sizes.sysDirFeedThru = (0);/* The model is not direct feedthrough */
  helicopter_p4_M->Sizes.numSampTimes = (2);/* Number of sample times */
  helicopter_p4_M->Sizes.numBlocks = (71);/* Number of blocks */
  helicopter_p4_M->Sizes.numBlockIO = (19);/* Number of block outputs */
  helicopter_p4_M->Sizes.numBlockPrms = (154);/* Sum of parameter "widths" */
  return helicopter_p4_M;
}

/*========================================================================*
 * End of Classic call interface                                          *
 *========================================================================*/
