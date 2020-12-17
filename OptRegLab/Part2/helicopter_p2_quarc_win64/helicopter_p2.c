/*
 * helicopter_p2.c
 *
 * Code generation for model "helicopter_p2".
 *
 * Model version              : 1.191
 * Simulink Coder version : 8.6 (R2014a) 27-Dec-2013
 * C source code generated on : Wed Feb 28 10:27:35 2018
 *
 * Target selection: quarc_win64.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: 32-bit Generic
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */
#include "helicopter_p2.h"
#include "helicopter_p2_private.h"
#include "helicopter_p2_dt.h"

/* Block signals (auto storage) */
B_helicopter_p2_T helicopter_p2_B;

/* Continuous states */
X_helicopter_p2_T helicopter_p2_X;

/* Block states (auto storage) */
DW_helicopter_p2_T helicopter_p2_DW;

/* Real-time model */
RT_MODEL_helicopter_p2_T helicopter_p2_M_;
RT_MODEL_helicopter_p2_T *const helicopter_p2_M = &helicopter_p2_M_;

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
  helicopter_p2_derivatives();
  rtsiSetT(si, tnew);
  for (i = 0; i < nXc; i++) {
    *x += h * f0[i];
    x++;
  }

  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

/* Model output function */
void helicopter_p2_output(void)
{
  /* local block i/o variables */
  real_T rtb_Backgain;
  real_T rtb_HILReadEncoderTimebase_o1;
  real_T rtb_HILReadEncoderTimebase_o2;
  real_T rtb_HILReadEncoderTimebase_o3;
  real_T *lastU;
  real_T rtb_Derivative;
  if (rtmIsMajorTimeStep(helicopter_p2_M)) {
    /* set solver stop time */
    if (!(helicopter_p2_M->Timing.clockTick0+1)) {
      rtsiSetSolverStopTime(&helicopter_p2_M->solverInfo,
                            ((helicopter_p2_M->Timing.clockTickH0 + 1) *
        helicopter_p2_M->Timing.stepSize0 * 4294967296.0));
    } else {
      rtsiSetSolverStopTime(&helicopter_p2_M->solverInfo,
                            ((helicopter_p2_M->Timing.clockTick0 + 1) *
        helicopter_p2_M->Timing.stepSize0 + helicopter_p2_M->Timing.clockTickH0 *
        helicopter_p2_M->Timing.stepSize0 * 4294967296.0));
    }
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(helicopter_p2_M)) {
    helicopter_p2_M->Timing.t[0] = rtsiGetT(&helicopter_p2_M->solverInfo);
  }

  if (rtmIsMajorTimeStep(helicopter_p2_M)) {
    /* S-Function (hil_read_encoder_timebase_block): '<S5>/HIL Read Encoder Timebase' */

    /* S-Function Block: helicopter_p2/Helicopter_interface/HIL Read Encoder Timebase (hil_read_encoder_timebase_block) */
    {
      t_error result;
      result = hil_task_read_encoder
        (helicopter_p2_DW.HILReadEncoderTimebase_Task, 1,
         &helicopter_p2_DW.HILReadEncoderTimebase_Buffer[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p2_M, _rt_error_message);
      } else {
        rtb_HILReadEncoderTimebase_o1 =
          helicopter_p2_DW.HILReadEncoderTimebase_Buffer[0];
        rtb_HILReadEncoderTimebase_o2 =
          helicopter_p2_DW.HILReadEncoderTimebase_Buffer[1];
        rtb_HILReadEncoderTimebase_o3 =
          helicopter_p2_DW.HILReadEncoderTimebase_Buffer[2];
      }
    }

    /* Gain: '<S5>/Elevation: Count to rad' */
    helicopter_p2_B.ElevationCounttorad =
      helicopter_p2_P.ElevationCounttorad_Gain * rtb_HILReadEncoderTimebase_o3;

    /* Gain: '<S8>/Gain' */
    helicopter_p2_B.Gain = helicopter_p2_P.Gain_Gain *
      helicopter_p2_B.ElevationCounttorad;

    /* Sum: '<Root>/Sum' incorporates:
     *  Constant: '<Root>/elavation_offset [deg]'
     */
    helicopter_p2_B.Sum = helicopter_p2_B.Gain +
      helicopter_p2_P.elavation_offsetdeg_Value;

    /* Gain: '<S5>/Pitch: Count to rad' */
    helicopter_p2_B.PitchCounttorad = helicopter_p2_P.PitchCounttorad_Gain *
      rtb_HILReadEncoderTimebase_o2;

    /* Gain: '<S10>/Gain' */
    helicopter_p2_B.Gain_i = helicopter_p2_P.Gain_Gain_a *
      helicopter_p2_B.PitchCounttorad;
  }

  /* FromWorkspace: '<Root>/Output from simulation' */
  {
    real_T *pDataValues = (real_T *)
      helicopter_p2_DW.Outputfromsimulation_PWORK.DataPtr;
    real_T *pTimeValues = (real_T *)
      helicopter_p2_DW.Outputfromsimulation_PWORK.TimePtr;
    int_T currTimeIndex = helicopter_p2_DW.Outputfromsimulation_IWORK.PrevIndex;
    real_T t = helicopter_p2_M->Timing.t[0];

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

    helicopter_p2_DW.Outputfromsimulation_IWORK.PrevIndex = currTimeIndex;

    /* Post output */
    {
      real_T t1 = pTimeValues[currTimeIndex];
      real_T t2 = pTimeValues[currTimeIndex + 1];
      if (t1 == t2) {
        if (t < t1) {
          helicopter_p2_B.Outputfromsimulation = pDataValues[currTimeIndex];
        } else {
          helicopter_p2_B.Outputfromsimulation = pDataValues[currTimeIndex + 1];
        }
      } else {
        real_T f1 = (t2 - t) / (t2 - t1);
        real_T f2 = 1.0 - f1;
        real_T d1;
        real_T d2;
        int_T TimeIndex= currTimeIndex;
        d1 = pDataValues[TimeIndex];
        d2 = pDataValues[TimeIndex + 1];
        helicopter_p2_B.Outputfromsimulation = (real_T) rtInterpolate(d1, d2, f1,
          f2);
        pDataValues += 141;
      }
    }
  }

  if (rtmIsMajorTimeStep(helicopter_p2_M)) {
    /* ToFile: '<Root>/To File3' */
    {
      if (!(++helicopter_p2_DW.ToFile3_IWORK.Decimation % 1) &&
          (helicopter_p2_DW.ToFile3_IWORK.Count*2)+1 < 100000000 ) {
        FILE *fp = (FILE *) helicopter_p2_DW.ToFile3_PWORK.FilePtr;
        if (fp != (NULL)) {
          real_T u[2];
          helicopter_p2_DW.ToFile3_IWORK.Decimation = 0;
          u[0] = helicopter_p2_M->Timing.t[1];
          u[1] = helicopter_p2_B.Outputfromsimulation;
          if (fwrite(u, sizeof(real_T), 2, fp) != 2) {
            rtmSetErrorStatus(helicopter_p2_M,
                              "Error writing to MAT-file p_ref_practical.mat");
            return;
          }

          if (((++helicopter_p2_DW.ToFile3_IWORK.Count)*2)+1 >= 100000000) {
            (void)fprintf(stdout,
                          "*** The ToFile block will stop logging data before\n"
                          "    the simulation has ended, because it has reached\n"
                          "    the maximum number of elements (100000000)\n"
                          "    allowed in MAT-file p_ref_practical.mat.\n");
          }
        }
      }
    }

    /* Gain: '<S5>/Travel: Count to rad' */
    helicopter_p2_B.TravelCounttorad = helicopter_p2_P.TravelCounttorad_Gain *
      rtb_HILReadEncoderTimebase_o1;

    /* Gain: '<S13>/Gain' */
    helicopter_p2_B.Gain_p = helicopter_p2_P.Gain_Gain_ar *
      helicopter_p2_B.TravelCounttorad;

    /* Sum: '<Root>/Sum3' incorporates:
     *  Constant: '<Root>/travel_offset [deg]'
     */
    helicopter_p2_B.Sum3 = helicopter_p2_B.Gain_p +
      helicopter_p2_P.travel_offsetdeg_Value;
  }

  /* TransferFcn: '<S5>/Travel: Transfer Fcn' */
  rtb_Backgain = 0.0;
  rtb_Backgain += helicopter_p2_P.TravelTransferFcn_C *
    helicopter_p2_X.TravelTransferFcn_CSTATE;
  rtb_Backgain += helicopter_p2_P.TravelTransferFcn_D *
    helicopter_p2_B.TravelCounttorad;

  /* Gain: '<S14>/Gain' */
  helicopter_p2_B.Gain_d = helicopter_p2_P.Gain_Gain_l * rtb_Backgain;
  if (rtmIsMajorTimeStep(helicopter_p2_M)) {
  }

  /* TransferFcn: '<S5>/Pitch: Transfer Fcn' */
  rtb_Backgain = 0.0;
  rtb_Backgain += helicopter_p2_P.PitchTransferFcn_C *
    helicopter_p2_X.PitchTransferFcn_CSTATE;
  rtb_Backgain += helicopter_p2_P.PitchTransferFcn_D *
    helicopter_p2_B.PitchCounttorad;

  /* Gain: '<S11>/Gain' */
  helicopter_p2_B.Gain_b = helicopter_p2_P.Gain_Gain_ae * rtb_Backgain;

  /* TransferFcn: '<S5>/Elevation: Transfer Fcn' */
  rtb_Backgain = 0.0;
  rtb_Backgain += helicopter_p2_P.ElevationTransferFcn_C *
    helicopter_p2_X.ElevationTransferFcn_CSTATE;
  rtb_Backgain += helicopter_p2_P.ElevationTransferFcn_D *
    helicopter_p2_B.ElevationCounttorad;

  /* Gain: '<S9>/Gain' */
  helicopter_p2_B.Gain_dg = helicopter_p2_P.Gain_Gain_n * rtb_Backgain;

  /* Gain: '<S3>/Gain1' */
  helicopter_p2_B.Gain1[0] = helicopter_p2_P.Gain1_Gain * helicopter_p2_B.Sum3;
  helicopter_p2_B.Gain1[1] = helicopter_p2_P.Gain1_Gain * helicopter_p2_B.Gain_d;
  helicopter_p2_B.Gain1[2] = helicopter_p2_P.Gain1_Gain * helicopter_p2_B.Gain_i;
  helicopter_p2_B.Gain1[3] = helicopter_p2_P.Gain1_Gain * helicopter_p2_B.Gain_b;
  helicopter_p2_B.Gain1[4] = helicopter_p2_P.Gain1_Gain * helicopter_p2_B.Sum;
  helicopter_p2_B.Gain1[5] = helicopter_p2_P.Gain1_Gain *
    helicopter_p2_B.Gain_dg;

  /* Sum: '<Root>/Sum1' incorporates:
   *  Constant: '<Root>/Vd_bias'
   *  Gain: '<S6>/K_pd'
   *  Gain: '<S6>/K_pp'
   *  Sum: '<S6>/Sum2'
   *  Sum: '<S6>/Sum3'
   */
  helicopter_p2_B.Sum1 = ((helicopter_p2_B.Outputfromsimulation -
    helicopter_p2_B.Gain1[2]) * helicopter_p2_P.K_pp - helicopter_p2_P.K_pd *
    helicopter_p2_B.Gain1[3]) + helicopter_p2_P.Vd_ff;
  if (rtmIsMajorTimeStep(helicopter_p2_M)) {
  }

  /* Integrator: '<S4>/Integrator'
   *
   * Regarding '<S4>/Integrator':
   *  Limited Integrator
   */
  if (helicopter_p2_X.Integrator_CSTATE >= helicopter_p2_P.Integrator_UpperSat )
  {
    helicopter_p2_X.Integrator_CSTATE = helicopter_p2_P.Integrator_UpperSat;
  } else if (helicopter_p2_X.Integrator_CSTATE <=
             (helicopter_p2_P.Integrator_LowerSat) ) {
    helicopter_p2_X.Integrator_CSTATE = (helicopter_p2_P.Integrator_LowerSat);
  }

  rtb_Backgain = helicopter_p2_X.Integrator_CSTATE;
  if (rtmIsMajorTimeStep(helicopter_p2_M)) {
    /* Gain: '<S2>/Gain1' incorporates:
     *  Constant: '<Root>/elevation_ref'
     */
    helicopter_p2_B.Gain1_f = helicopter_p2_P.Gain1_Gain_m *
      helicopter_p2_P.elevation_ref_Value;
  }

  /* Sum: '<S4>/Sum' */
  rtb_Derivative = helicopter_p2_B.Gain1_f - helicopter_p2_B.Gain1[4];

  /* Sum: '<Root>/Sum2' incorporates:
   *  Constant: '<Root>/Vs_bias'
   *  Gain: '<S4>/K_ed'
   *  Gain: '<S4>/K_ep'
   *  Sum: '<S4>/Sum1'
   */
  helicopter_p2_B.Sum2 = ((helicopter_p2_P.K_ep * rtb_Derivative + rtb_Backgain)
    - helicopter_p2_P.K_ed * helicopter_p2_B.Gain1[5]) + helicopter_p2_P.Vs_ff;
  if (rtmIsMajorTimeStep(helicopter_p2_M)) {
    /* ToFile: '<S7>/To File' */
    {
      if (!(++helicopter_p2_DW.ToFile_IWORK.Decimation % 1) &&
          (helicopter_p2_DW.ToFile_IWORK.Count*2)+1 < 100000000 ) {
        FILE *fp = (FILE *) helicopter_p2_DW.ToFile_PWORK.FilePtr;
        if (fp != (NULL)) {
          real_T u[2];
          helicopter_p2_DW.ToFile_IWORK.Decimation = 0;
          u[0] = helicopter_p2_M->Timing.t[1];
          u[1] = helicopter_p2_B.Gain1[3];
          if (fwrite(u, sizeof(real_T), 2, fp) != 2) {
            rtmSetErrorStatus(helicopter_p2_M,
                              "Error writing to MAT-file p_dot_practical.mat");
            return;
          }

          if (((++helicopter_p2_DW.ToFile_IWORK.Count)*2)+1 >= 100000000) {
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
      if (!(++helicopter_p2_DW.ToFile1_IWORK.Decimation % 1) &&
          (helicopter_p2_DW.ToFile1_IWORK.Count*2)+1 < 100000000 ) {
        FILE *fp = (FILE *) helicopter_p2_DW.ToFile1_PWORK.FilePtr;
        if (fp != (NULL)) {
          real_T u[2];
          helicopter_p2_DW.ToFile1_IWORK.Decimation = 0;
          u[0] = helicopter_p2_M->Timing.t[1];
          u[1] = helicopter_p2_B.Gain1[2];
          if (fwrite(u, sizeof(real_T), 2, fp) != 2) {
            rtmSetErrorStatus(helicopter_p2_M,
                              "Error writing to MAT-file p_practical.mat");
            return;
          }

          if (((++helicopter_p2_DW.ToFile1_IWORK.Count)*2)+1 >= 100000000) {
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
      if (!(++helicopter_p2_DW.ToFile2_IWORK.Decimation % 1) &&
          (helicopter_p2_DW.ToFile2_IWORK.Count*2)+1 < 100000000 ) {
        FILE *fp = (FILE *) helicopter_p2_DW.ToFile2_PWORK.FilePtr;
        if (fp != (NULL)) {
          real_T u[2];
          helicopter_p2_DW.ToFile2_IWORK.Decimation = 0;
          u[0] = helicopter_p2_M->Timing.t[1];
          u[1] = helicopter_p2_B.Gain1[0];
          if (fwrite(u, sizeof(real_T), 2, fp) != 2) {
            rtmSetErrorStatus(helicopter_p2_M,
                              "Error writing to MAT-file lambda_practical.mat");
            return;
          }

          if (((++helicopter_p2_DW.ToFile2_IWORK.Count)*2)+1 >= 100000000) {
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
      if (!(++helicopter_p2_DW.ToFile4_IWORK.Decimation % 1) &&
          (helicopter_p2_DW.ToFile4_IWORK.Count*2)+1 < 100000000 ) {
        FILE *fp = (FILE *) helicopter_p2_DW.ToFile4_PWORK.FilePtr;
        if (fp != (NULL)) {
          real_T u[2];
          helicopter_p2_DW.ToFile4_IWORK.Decimation = 0;
          u[0] = helicopter_p2_M->Timing.t[1];
          u[1] = helicopter_p2_B.Gain1[1];
          if (fwrite(u, sizeof(real_T), 2, fp) != 2) {
            rtmSetErrorStatus(helicopter_p2_M,
                              "Error writing to MAT-file r_practical.mat");
            return;
          }

          if (((++helicopter_p2_DW.ToFile4_IWORK.Count)*2)+1 >= 100000000) {
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
  rtb_Backgain = (helicopter_p2_B.Sum2 - helicopter_p2_B.Sum1) *
    helicopter_p2_P.Backgain_Gain;

  /* Gain: '<S4>/K_ei' */
  helicopter_p2_B.K_ei = helicopter_p2_P.K_ei * rtb_Derivative;
  if (rtmIsMajorTimeStep(helicopter_p2_M)) {
  }

  /* Derivative: '<S5>/Derivative' */
  if ((helicopter_p2_DW.TimeStampA >= helicopter_p2_M->Timing.t[0]) &&
      (helicopter_p2_DW.TimeStampB >= helicopter_p2_M->Timing.t[0])) {
    rtb_Derivative = 0.0;
  } else {
    rtb_Derivative = helicopter_p2_DW.TimeStampA;
    lastU = &helicopter_p2_DW.LastUAtTimeA;
    if (helicopter_p2_DW.TimeStampA < helicopter_p2_DW.TimeStampB) {
      if (helicopter_p2_DW.TimeStampB < helicopter_p2_M->Timing.t[0]) {
        rtb_Derivative = helicopter_p2_DW.TimeStampB;
        lastU = &helicopter_p2_DW.LastUAtTimeB;
      }
    } else {
      if (helicopter_p2_DW.TimeStampA >= helicopter_p2_M->Timing.t[0]) {
        rtb_Derivative = helicopter_p2_DW.TimeStampB;
        lastU = &helicopter_p2_DW.LastUAtTimeB;
      }
    }

    rtb_Derivative = (helicopter_p2_B.PitchCounttorad - *lastU) /
      (helicopter_p2_M->Timing.t[0] - rtb_Derivative);
  }

  /* End of Derivative: '<S5>/Derivative' */

  /* Gain: '<S12>/Gain' */
  helicopter_p2_B.Gain_l = helicopter_p2_P.Gain_Gain_a1 * rtb_Derivative;
  if (rtmIsMajorTimeStep(helicopter_p2_M)) {
  }

  /* Saturate: '<S5>/Back motor: Saturation' */
  if (rtb_Backgain > helicopter_p2_P.BackmotorSaturation_UpperSat) {
    helicopter_p2_B.BackmotorSaturation =
      helicopter_p2_P.BackmotorSaturation_UpperSat;
  } else if (rtb_Backgain < helicopter_p2_P.BackmotorSaturation_LowerSat) {
    helicopter_p2_B.BackmotorSaturation =
      helicopter_p2_P.BackmotorSaturation_LowerSat;
  } else {
    helicopter_p2_B.BackmotorSaturation = rtb_Backgain;
  }

  /* End of Saturate: '<S5>/Back motor: Saturation' */
  if (rtmIsMajorTimeStep(helicopter_p2_M)) {
  }

  /* Gain: '<S1>/Front gain' incorporates:
   *  Sum: '<S1>/Add'
   */
  rtb_Derivative = (helicopter_p2_B.Sum1 + helicopter_p2_B.Sum2) *
    helicopter_p2_P.Frontgain_Gain;

  /* Saturate: '<S5>/Front motor: Saturation' */
  if (rtb_Derivative > helicopter_p2_P.FrontmotorSaturation_UpperSat) {
    helicopter_p2_B.FrontmotorSaturation =
      helicopter_p2_P.FrontmotorSaturation_UpperSat;
  } else if (rtb_Derivative < helicopter_p2_P.FrontmotorSaturation_LowerSat) {
    helicopter_p2_B.FrontmotorSaturation =
      helicopter_p2_P.FrontmotorSaturation_LowerSat;
  } else {
    helicopter_p2_B.FrontmotorSaturation = rtb_Derivative;
  }

  /* End of Saturate: '<S5>/Front motor: Saturation' */
  if (rtmIsMajorTimeStep(helicopter_p2_M)) {
    /* S-Function (hil_write_analog_block): '<S5>/HIL Write Analog' */

    /* S-Function Block: helicopter_p2/Helicopter_interface/HIL Write Analog (hil_write_analog_block) */
    {
      t_error result;
      helicopter_p2_DW.HILWriteAnalog_Buffer[0] =
        helicopter_p2_B.FrontmotorSaturation;
      helicopter_p2_DW.HILWriteAnalog_Buffer[1] =
        helicopter_p2_B.BackmotorSaturation;
      result = hil_write_analog(helicopter_p2_DW.HILInitialize_Card,
        helicopter_p2_P.HILWriteAnalog_channels, 2,
        &helicopter_p2_DW.HILWriteAnalog_Buffer[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p2_M, _rt_error_message);
      }
    }
  }
}

/* Model update function */
void helicopter_p2_update(void)
{
  real_T *lastU;

  /* Update for Derivative: '<S5>/Derivative' */
  if (helicopter_p2_DW.TimeStampA == (rtInf)) {
    helicopter_p2_DW.TimeStampA = helicopter_p2_M->Timing.t[0];
    lastU = &helicopter_p2_DW.LastUAtTimeA;
  } else if (helicopter_p2_DW.TimeStampB == (rtInf)) {
    helicopter_p2_DW.TimeStampB = helicopter_p2_M->Timing.t[0];
    lastU = &helicopter_p2_DW.LastUAtTimeB;
  } else if (helicopter_p2_DW.TimeStampA < helicopter_p2_DW.TimeStampB) {
    helicopter_p2_DW.TimeStampA = helicopter_p2_M->Timing.t[0];
    lastU = &helicopter_p2_DW.LastUAtTimeA;
  } else {
    helicopter_p2_DW.TimeStampB = helicopter_p2_M->Timing.t[0];
    lastU = &helicopter_p2_DW.LastUAtTimeB;
  }

  *lastU = helicopter_p2_B.PitchCounttorad;

  /* End of Update for Derivative: '<S5>/Derivative' */
  if (rtmIsMajorTimeStep(helicopter_p2_M)) {
    rt_ertODEUpdateContinuousStates(&helicopter_p2_M->solverInfo);
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
  if (!(++helicopter_p2_M->Timing.clockTick0)) {
    ++helicopter_p2_M->Timing.clockTickH0;
  }

  helicopter_p2_M->Timing.t[0] = rtsiGetSolverStopTime
    (&helicopter_p2_M->solverInfo);

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
    if (!(++helicopter_p2_M->Timing.clockTick1)) {
      ++helicopter_p2_M->Timing.clockTickH1;
    }

    helicopter_p2_M->Timing.t[1] = helicopter_p2_M->Timing.clockTick1 *
      helicopter_p2_M->Timing.stepSize1 + helicopter_p2_M->Timing.clockTickH1 *
      helicopter_p2_M->Timing.stepSize1 * 4294967296.0;
  }
}

/* Derivatives for root system: '<Root>' */
void helicopter_p2_derivatives(void)
{
  XDot_helicopter_p2_T *_rtXdot;
  _rtXdot = ((XDot_helicopter_p2_T *) helicopter_p2_M->ModelData.derivs);

  /* Derivatives for TransferFcn: '<S5>/Travel: Transfer Fcn' */
  _rtXdot->TravelTransferFcn_CSTATE = 0.0;
  _rtXdot->TravelTransferFcn_CSTATE += helicopter_p2_P.TravelTransferFcn_A *
    helicopter_p2_X.TravelTransferFcn_CSTATE;
  _rtXdot->TravelTransferFcn_CSTATE += helicopter_p2_B.TravelCounttorad;

  /* Derivatives for TransferFcn: '<S5>/Pitch: Transfer Fcn' */
  _rtXdot->PitchTransferFcn_CSTATE = 0.0;
  _rtXdot->PitchTransferFcn_CSTATE += helicopter_p2_P.PitchTransferFcn_A *
    helicopter_p2_X.PitchTransferFcn_CSTATE;
  _rtXdot->PitchTransferFcn_CSTATE += helicopter_p2_B.PitchCounttorad;

  /* Derivatives for TransferFcn: '<S5>/Elevation: Transfer Fcn' */
  _rtXdot->ElevationTransferFcn_CSTATE = 0.0;
  _rtXdot->ElevationTransferFcn_CSTATE += helicopter_p2_P.ElevationTransferFcn_A
    * helicopter_p2_X.ElevationTransferFcn_CSTATE;
  _rtXdot->ElevationTransferFcn_CSTATE += helicopter_p2_B.ElevationCounttorad;

  /* Derivatives for Integrator: '<S4>/Integrator' */
  {
    boolean_T lsat;
    boolean_T usat;
    lsat = ( helicopter_p2_X.Integrator_CSTATE <=
            (helicopter_p2_P.Integrator_LowerSat) );
    usat = ( helicopter_p2_X.Integrator_CSTATE >=
            helicopter_p2_P.Integrator_UpperSat );
    if ((!lsat && !usat) ||
        (lsat && (helicopter_p2_B.K_ei > 0)) ||
        (usat && (helicopter_p2_B.K_ei < 0)) ) {
      ((XDot_helicopter_p2_T *) helicopter_p2_M->ModelData.derivs)
        ->Integrator_CSTATE = helicopter_p2_B.K_ei;
    } else {
      /* in saturation */
      ((XDot_helicopter_p2_T *) helicopter_p2_M->ModelData.derivs)
        ->Integrator_CSTATE = 0.0;
    }
  }
}

/* Model initialize function */
void helicopter_p2_initialize(void)
{
  /* Start for S-Function (hil_initialize_block): '<Root>/HIL Initialize' */

  /* S-Function Block: helicopter_p2/HIL Initialize (hil_initialize_block) */
  {
    t_int result;
    t_boolean is_switching;
    result = hil_open("q8_usb", "0", &helicopter_p2_DW.HILInitialize_Card);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(helicopter_p2_M, _rt_error_message);
      return;
    }

    is_switching = false;
    result = hil_set_card_specific_options(helicopter_p2_DW.HILInitialize_Card,
      "update_rate=normal;decimation=1", 32);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(helicopter_p2_M, _rt_error_message);
      return;
    }

    result = hil_watchdog_clear(helicopter_p2_DW.HILInitialize_Card);
    if (result < 0 && result != -QERR_HIL_WATCHDOG_CLEAR) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(helicopter_p2_M, _rt_error_message);
      return;
    }

    if ((helicopter_p2_P.HILInitialize_set_analog_input_ && !is_switching) ||
        (helicopter_p2_P.HILInitialize_set_analog_inpu_m && is_switching)) {
      {
        int_T i1;
        real_T *dw_AIMinimums = &helicopter_p2_DW.HILInitialize_AIMinimums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AIMinimums[i1] = helicopter_p2_P.HILInitialize_analog_input_mini;
        }
      }

      {
        int_T i1;
        real_T *dw_AIMaximums = &helicopter_p2_DW.HILInitialize_AIMaximums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AIMaximums[i1] = helicopter_p2_P.HILInitialize_analog_input_maxi;
        }
      }

      result = hil_set_analog_input_ranges(helicopter_p2_DW.HILInitialize_Card,
        helicopter_p2_P.HILInitialize_analog_input_chan, 8U,
        &helicopter_p2_DW.HILInitialize_AIMinimums[0],
        &helicopter_p2_DW.HILInitialize_AIMaximums[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p2_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_p2_P.HILInitialize_set_analog_output && !is_switching) ||
        (helicopter_p2_P.HILInitialize_set_analog_outp_b && is_switching)) {
      {
        int_T i1;
        real_T *dw_AOMinimums = &helicopter_p2_DW.HILInitialize_AOMinimums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOMinimums[i1] = helicopter_p2_P.HILInitialize_analog_output_min;
        }
      }

      {
        int_T i1;
        real_T *dw_AOMaximums = &helicopter_p2_DW.HILInitialize_AOMaximums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOMaximums[i1] = helicopter_p2_P.HILInitialize_analog_output_max;
        }
      }

      result = hil_set_analog_output_ranges(helicopter_p2_DW.HILInitialize_Card,
        helicopter_p2_P.HILInitialize_analog_output_cha, 8U,
        &helicopter_p2_DW.HILInitialize_AOMinimums[0],
        &helicopter_p2_DW.HILInitialize_AOMaximums[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p2_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_p2_P.HILInitialize_set_analog_outp_e && !is_switching) ||
        (helicopter_p2_P.HILInitialize_set_analog_outp_j && is_switching)) {
      {
        int_T i1;
        real_T *dw_AOVoltages = &helicopter_p2_DW.HILInitialize_AOVoltages[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOVoltages[i1] = helicopter_p2_P.HILInitialize_initial_analog_ou;
        }
      }

      result = hil_write_analog(helicopter_p2_DW.HILInitialize_Card,
        helicopter_p2_P.HILInitialize_analog_output_cha, 8U,
        &helicopter_p2_DW.HILInitialize_AOVoltages[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p2_M, _rt_error_message);
        return;
      }
    }

    if (helicopter_p2_P.HILInitialize_set_analog_outp_p) {
      {
        int_T i1;
        real_T *dw_AOVoltages = &helicopter_p2_DW.HILInitialize_AOVoltages[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOVoltages[i1] = helicopter_p2_P.HILInitialize_watchdog_analog_o;
        }
      }

      result = hil_watchdog_set_analog_expiration_state
        (helicopter_p2_DW.HILInitialize_Card,
         helicopter_p2_P.HILInitialize_analog_output_cha, 8U,
         &helicopter_p2_DW.HILInitialize_AOVoltages[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p2_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_p2_P.HILInitialize_set_encoder_param && !is_switching) ||
        (helicopter_p2_P.HILInitialize_set_encoder_par_m && is_switching)) {
      {
        int_T i1;
        int32_T *dw_QuadratureModes =
          &helicopter_p2_DW.HILInitialize_QuadratureModes[0];
        for (i1=0; i1 < 8; i1++) {
          dw_QuadratureModes[i1] = helicopter_p2_P.HILInitialize_quadrature;
        }
      }

      result = hil_set_encoder_quadrature_mode
        (helicopter_p2_DW.HILInitialize_Card,
         helicopter_p2_P.HILInitialize_encoder_channels, 8U,
         (t_encoder_quadrature_mode *)
         &helicopter_p2_DW.HILInitialize_QuadratureModes[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p2_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_p2_P.HILInitialize_set_encoder_count && !is_switching) ||
        (helicopter_p2_P.HILInitialize_set_encoder_cou_k && is_switching)) {
      {
        int_T i1;
        int32_T *dw_InitialEICounts =
          &helicopter_p2_DW.HILInitialize_InitialEICounts[0];
        for (i1=0; i1 < 8; i1++) {
          dw_InitialEICounts[i1] =
            helicopter_p2_P.HILInitialize_initial_encoder_c;
        }
      }

      result = hil_set_encoder_counts(helicopter_p2_DW.HILInitialize_Card,
        helicopter_p2_P.HILInitialize_encoder_channels, 8U,
        &helicopter_p2_DW.HILInitialize_InitialEICounts[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p2_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_p2_P.HILInitialize_set_pwm_params_at && !is_switching) ||
        (helicopter_p2_P.HILInitialize_set_pwm_params__f && is_switching)) {
      uint32_T num_duty_cycle_modes = 0;
      uint32_T num_frequency_modes = 0;

      {
        int_T i1;
        int32_T *dw_POModeValues = &helicopter_p2_DW.HILInitialize_POModeValues
          [0];
        for (i1=0; i1 < 8; i1++) {
          dw_POModeValues[i1] = helicopter_p2_P.HILInitialize_pwm_modes;
        }
      }

      result = hil_set_pwm_mode(helicopter_p2_DW.HILInitialize_Card,
        helicopter_p2_P.HILInitialize_pwm_channels, 8U, (t_pwm_mode *)
        &helicopter_p2_DW.HILInitialize_POModeValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p2_M, _rt_error_message);
        return;
      }

      {
        int_T i1;
        const uint32_T *p_HILInitialize_pwm_channels =
          helicopter_p2_P.HILInitialize_pwm_channels;
        int32_T *dw_POModeValues = &helicopter_p2_DW.HILInitialize_POModeValues
          [0];
        for (i1=0; i1 < 8; i1++) {
          if (dw_POModeValues[i1] == PWM_DUTY_CYCLE_MODE || dw_POModeValues[i1] ==
              PWM_ONE_SHOT_MODE || dw_POModeValues[i1] == PWM_TIME_MODE) {
            helicopter_p2_DW.HILInitialize_POSortedChans[num_duty_cycle_modes] =
              p_HILInitialize_pwm_channels[i1];
            helicopter_p2_DW.HILInitialize_POSortedFreqs[num_duty_cycle_modes] =
              helicopter_p2_P.HILInitialize_pwm_frequency;
            num_duty_cycle_modes++;
          } else {
            helicopter_p2_DW.HILInitialize_POSortedChans[7U -
              num_frequency_modes] = p_HILInitialize_pwm_channels[i1];
            helicopter_p2_DW.HILInitialize_POSortedFreqs[7U -
              num_frequency_modes] = helicopter_p2_P.HILInitialize_pwm_frequency;
            num_frequency_modes++;
          }
        }
      }

      if (num_duty_cycle_modes > 0) {
        result = hil_set_pwm_frequency(helicopter_p2_DW.HILInitialize_Card,
          &helicopter_p2_DW.HILInitialize_POSortedChans[0], num_duty_cycle_modes,
          &helicopter_p2_DW.HILInitialize_POSortedFreqs[0]);
        if (result < 0) {
          msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
            (_rt_error_message));
          rtmSetErrorStatus(helicopter_p2_M, _rt_error_message);
          return;
        }
      }

      if (num_frequency_modes > 0) {
        result = hil_set_pwm_duty_cycle(helicopter_p2_DW.HILInitialize_Card,
          &helicopter_p2_DW.HILInitialize_POSortedChans[num_duty_cycle_modes],
          num_frequency_modes,
          &helicopter_p2_DW.HILInitialize_POSortedFreqs[num_duty_cycle_modes]);
        if (result < 0) {
          msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
            (_rt_error_message));
          rtmSetErrorStatus(helicopter_p2_M, _rt_error_message);
          return;
        }
      }

      {
        int_T i1;
        int32_T *dw_POModeValues = &helicopter_p2_DW.HILInitialize_POModeValues
          [0];
        for (i1=0; i1 < 8; i1++) {
          dw_POModeValues[i1] = helicopter_p2_P.HILInitialize_pwm_configuration;
        }
      }

      {
        int_T i1;
        int32_T *dw_POAlignValues =
          &helicopter_p2_DW.HILInitialize_POAlignValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POAlignValues[i1] = helicopter_p2_P.HILInitialize_pwm_alignment;
        }
      }

      {
        int_T i1;
        int32_T *dw_POPolarityVals =
          &helicopter_p2_DW.HILInitialize_POPolarityVals[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POPolarityVals[i1] = helicopter_p2_P.HILInitialize_pwm_polarity;
        }
      }

      result = hil_set_pwm_configuration(helicopter_p2_DW.HILInitialize_Card,
        helicopter_p2_P.HILInitialize_pwm_channels, 8U,
        (t_pwm_configuration *) &helicopter_p2_DW.HILInitialize_POModeValues[0],
        (t_pwm_alignment *) &helicopter_p2_DW.HILInitialize_POAlignValues[0],
        (t_pwm_polarity *) &helicopter_p2_DW.HILInitialize_POPolarityVals[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p2_M, _rt_error_message);
        return;
      }

      {
        int_T i1;
        real_T *dw_POSortedFreqs =
          &helicopter_p2_DW.HILInitialize_POSortedFreqs[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POSortedFreqs[i1] = helicopter_p2_P.HILInitialize_pwm_leading_deadb;
        }
      }

      {
        int_T i1;
        real_T *dw_POValues = &helicopter_p2_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = helicopter_p2_P.HILInitialize_pwm_trailing_dead;
        }
      }

      result = hil_set_pwm_deadband(helicopter_p2_DW.HILInitialize_Card,
        helicopter_p2_P.HILInitialize_pwm_channels, 8U,
        &helicopter_p2_DW.HILInitialize_POSortedFreqs[0],
        &helicopter_p2_DW.HILInitialize_POValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p2_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_p2_P.HILInitialize_set_pwm_outputs_a && !is_switching) ||
        (helicopter_p2_P.HILInitialize_set_pwm_outputs_g && is_switching)) {
      {
        int_T i1;
        real_T *dw_POValues = &helicopter_p2_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = helicopter_p2_P.HILInitialize_initial_pwm_outpu;
        }
      }

      result = hil_write_pwm(helicopter_p2_DW.HILInitialize_Card,
        helicopter_p2_P.HILInitialize_pwm_channels, 8U,
        &helicopter_p2_DW.HILInitialize_POValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p2_M, _rt_error_message);
        return;
      }
    }

    if (helicopter_p2_P.HILInitialize_set_pwm_outputs_o) {
      {
        int_T i1;
        real_T *dw_POValues = &helicopter_p2_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = helicopter_p2_P.HILInitialize_watchdog_pwm_outp;
        }
      }

      result = hil_watchdog_set_pwm_expiration_state
        (helicopter_p2_DW.HILInitialize_Card,
         helicopter_p2_P.HILInitialize_pwm_channels, 8U,
         &helicopter_p2_DW.HILInitialize_POValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p2_M, _rt_error_message);
        return;
      }
    }
  }

  /* Start for S-Function (hil_read_encoder_timebase_block): '<S5>/HIL Read Encoder Timebase' */

  /* S-Function Block: helicopter_p2/Helicopter_interface/HIL Read Encoder Timebase (hil_read_encoder_timebase_block) */
  {
    t_error result;
    result = hil_task_create_encoder_reader(helicopter_p2_DW.HILInitialize_Card,
      helicopter_p2_P.HILReadEncoderTimebase_samples_,
      helicopter_p2_P.HILReadEncoderTimebase_channels, 3,
      &helicopter_p2_DW.HILReadEncoderTimebase_Task);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(helicopter_p2_M, _rt_error_message);
    }
  }

  /* Start for FromWorkspace: '<Root>/Output from simulation' */
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
      -0.52359877559826418, -0.52359877559810264, -0.52359877559822143,
      -0.52359877559707113, -0.52359877524912035, -0.48189430424436047,
      -0.383650232258344, -0.29491318995915511, -0.21528809690500478,
      -0.14434491264357996, -0.081625804744567917, -0.026652380707167831,
      0.02106735703553023, 0.062032548936278659, 0.0967436280055929,
      0.12569713469535138, 0.14938120764583238, 0.16827173414486768,
      0.18282912797186132, 0.19349569306591241, 0.2006935266963559,
      0.20482291391153579, 0.20626116495829222, 0.20536184843592384,
      0.20245437474907982, 0.19784388667996958, 0.19181141643197208,
      0.18461427118412374, 0.17648661195658807, 0.16764019336273567,
      0.15826523457150252, 0.14853139449273209, 0.13858882680447704,
      0.12856929294673325, 0.11858731359727324, 0.10874134141220419,
      0.099114939949640074, 0.089777955694994, 0.080787671968531607,
      0.0721899352194664, 0.064020245797047848, 0.05630480674013072,
      0.04906152544604242, 0.0423009642715243, 0.036027237188173647,
      0.030238850567802757, 0.024929487015543345, 0.020088731906758528,
      0.0157027429244877, 0.011754863443956067, 0.0082261810763717743,
      0.0050960330724998384, 0.0023424606039241886, -5.738580709711798E-5,
      -0.0021268872435054177, -0.003889641126849387, -0.0053691870240822675,
      -0.0065887707688201593, -0.0075711441194299643, -0.0083383971889239555,
      -0.0089118209211151451, -0.0093117969497178554, -0.0095577122582693446,
      -0.00966789615542962, -0.0096595771892266646, -0.0095488577422947283,
      -0.0093507041755340071, -0.009078950517608252, -0.00874631383025964,
      -0.0083644195127709122, -0.0079438349414831209, -0.0074941099707316111,
      -0.0070238229487262927, -0.0065406310247668577, -0.0060513236418641876,
      -0.0055618782205259943, -0.0050775171443515447, -0.0046027652552560593,
      -0.0041415071544223326, -0.0036970436827162131, -0.0032721470185726438,
      -0.0028691138778321004, -0.0024898163214455917, -0.0021357496615932265,
      -0.0018080768856509647, -0.0015076688607286303, -0.001235139293240385,
      -0.0009908729299193625, -0.00077504470279178518, -0.00058762631738011259,
      -0.00042837502954633531, -0.00029679698322671017, -0.00019207467346200204,
      -0.00011294575559975468, -5.7521201996700027E-5, -2.3041412711169529E-5,
      -5.6048922177185367E-6, 1.596760991010176E-15, 1.5916774997421879E-15, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0 } ;

    helicopter_p2_DW.Outputfromsimulation_PWORK.TimePtr = (void *) pTimeValues0;
    helicopter_p2_DW.Outputfromsimulation_PWORK.DataPtr = (void *) pDataValues0;
    helicopter_p2_DW.Outputfromsimulation_IWORK.PrevIndex = 0;
  }

  /* Start for ToFile: '<Root>/To File3' */
  {
    char fileName[509] = "p_ref_practical.mat";
    FILE *fp = (NULL);
    if ((fp = fopen(fileName, "wb")) == (NULL)) {
      rtmSetErrorStatus(helicopter_p2_M,
                        "Error creating .mat file p_ref_practical.mat");
      return;
    }

    if (rt_WriteMat4FileHeader(fp,2,0,"p_ref")) {
      rtmSetErrorStatus(helicopter_p2_M,
                        "Error writing mat file header to file p_ref_practical.mat");
      return;
    }

    helicopter_p2_DW.ToFile3_IWORK.Count = 0;
    helicopter_p2_DW.ToFile3_IWORK.Decimation = -1;
    helicopter_p2_DW.ToFile3_PWORK.FilePtr = fp;
  }

  /* Start for ToFile: '<S7>/To File' */
  {
    char fileName[509] = "p_dot_practical.mat";
    FILE *fp = (NULL);
    if ((fp = fopen(fileName, "wb")) == (NULL)) {
      rtmSetErrorStatus(helicopter_p2_M,
                        "Error creating .mat file p_dot_practical.mat");
      return;
    }

    if (rt_WriteMat4FileHeader(fp,2,0,"p_dot")) {
      rtmSetErrorStatus(helicopter_p2_M,
                        "Error writing mat file header to file p_dot_practical.mat");
      return;
    }

    helicopter_p2_DW.ToFile_IWORK.Count = 0;
    helicopter_p2_DW.ToFile_IWORK.Decimation = -1;
    helicopter_p2_DW.ToFile_PWORK.FilePtr = fp;
  }

  /* Start for ToFile: '<S7>/To File1' */
  {
    char fileName[509] = "p_practical.mat";
    FILE *fp = (NULL);
    if ((fp = fopen(fileName, "wb")) == (NULL)) {
      rtmSetErrorStatus(helicopter_p2_M,
                        "Error creating .mat file p_practical.mat");
      return;
    }

    if (rt_WriteMat4FileHeader(fp,2,0,"p")) {
      rtmSetErrorStatus(helicopter_p2_M,
                        "Error writing mat file header to file p_practical.mat");
      return;
    }

    helicopter_p2_DW.ToFile1_IWORK.Count = 0;
    helicopter_p2_DW.ToFile1_IWORK.Decimation = -1;
    helicopter_p2_DW.ToFile1_PWORK.FilePtr = fp;
  }

  /* Start for ToFile: '<S7>/To File2' */
  {
    char fileName[509] = "lambda_practical.mat";
    FILE *fp = (NULL);
    if ((fp = fopen(fileName, "wb")) == (NULL)) {
      rtmSetErrorStatus(helicopter_p2_M,
                        "Error creating .mat file lambda_practical.mat");
      return;
    }

    if (rt_WriteMat4FileHeader(fp,2,0,"lambda")) {
      rtmSetErrorStatus(helicopter_p2_M,
                        "Error writing mat file header to file lambda_practical.mat");
      return;
    }

    helicopter_p2_DW.ToFile2_IWORK.Count = 0;
    helicopter_p2_DW.ToFile2_IWORK.Decimation = -1;
    helicopter_p2_DW.ToFile2_PWORK.FilePtr = fp;
  }

  /* Start for ToFile: '<S7>/To File4' */
  {
    char fileName[509] = "r_practical.mat";
    FILE *fp = (NULL);
    if ((fp = fopen(fileName, "wb")) == (NULL)) {
      rtmSetErrorStatus(helicopter_p2_M,
                        "Error creating .mat file r_practical.mat");
      return;
    }

    if (rt_WriteMat4FileHeader(fp,2,0,"r")) {
      rtmSetErrorStatus(helicopter_p2_M,
                        "Error writing mat file header to file r_practical.mat");
      return;
    }

    helicopter_p2_DW.ToFile4_IWORK.Count = 0;
    helicopter_p2_DW.ToFile4_IWORK.Decimation = -1;
    helicopter_p2_DW.ToFile4_PWORK.FilePtr = fp;
  }

  /* InitializeConditions for TransferFcn: '<S5>/Travel: Transfer Fcn' */
  helicopter_p2_X.TravelTransferFcn_CSTATE = 0.0;

  /* InitializeConditions for TransferFcn: '<S5>/Pitch: Transfer Fcn' */
  helicopter_p2_X.PitchTransferFcn_CSTATE = 0.0;

  /* InitializeConditions for TransferFcn: '<S5>/Elevation: Transfer Fcn' */
  helicopter_p2_X.ElevationTransferFcn_CSTATE = 0.0;

  /* InitializeConditions for Integrator: '<S4>/Integrator' */
  helicopter_p2_X.Integrator_CSTATE = helicopter_p2_P.Integrator_IC;

  /* InitializeConditions for Derivative: '<S5>/Derivative' */
  helicopter_p2_DW.TimeStampA = (rtInf);
  helicopter_p2_DW.TimeStampB = (rtInf);
}

/* Model terminate function */
void helicopter_p2_terminate(void)
{
  /* Terminate for S-Function (hil_initialize_block): '<Root>/HIL Initialize' */

  /* S-Function Block: helicopter_p2/HIL Initialize (hil_initialize_block) */
  {
    t_boolean is_switching;
    t_int result;
    t_uint32 num_final_analog_outputs = 0;
    t_uint32 num_final_pwm_outputs = 0;
    hil_task_stop_all(helicopter_p2_DW.HILInitialize_Card);
    hil_monitor_stop_all(helicopter_p2_DW.HILInitialize_Card);
    is_switching = false;
    if ((helicopter_p2_P.HILInitialize_set_analog_out_ex && !is_switching) ||
        (helicopter_p2_P.HILInitialize_set_analog_outp_c && is_switching)) {
      {
        int_T i1;
        real_T *dw_AOVoltages = &helicopter_p2_DW.HILInitialize_AOVoltages[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOVoltages[i1] = helicopter_p2_P.HILInitialize_final_analog_outp;
        }
      }

      num_final_analog_outputs = 8U;
    }

    if ((helicopter_p2_P.HILInitialize_set_pwm_output_ap && !is_switching) ||
        (helicopter_p2_P.HILInitialize_set_pwm_outputs_p && is_switching)) {
      {
        int_T i1;
        real_T *dw_POValues = &helicopter_p2_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = helicopter_p2_P.HILInitialize_final_pwm_outputs;
        }
      }

      num_final_pwm_outputs = 8U;
    }

    if (0
        || num_final_analog_outputs > 0
        || num_final_pwm_outputs > 0
        ) {
      /* Attempt to write the final outputs atomically (due to firmware issue in old Q2-USB). Otherwise write channels individually */
      result = hil_write(helicopter_p2_DW.HILInitialize_Card
                         , helicopter_p2_P.HILInitialize_analog_output_cha,
                         num_final_analog_outputs
                         , helicopter_p2_P.HILInitialize_pwm_channels,
                         num_final_pwm_outputs
                         , NULL, 0
                         , NULL, 0
                         , &helicopter_p2_DW.HILInitialize_AOVoltages[0]
                         , &helicopter_p2_DW.HILInitialize_POValues[0]
                         , (t_boolean *) NULL
                         , NULL
                         );
      if (result == -QERR_HIL_WRITE_NOT_SUPPORTED) {
        t_error local_result;
        result = 0;

        /* The hil_write operation is not supported by this card. Write final outputs for each channel type */
        if (num_final_analog_outputs > 0) {
          local_result = hil_write_analog(helicopter_p2_DW.HILInitialize_Card,
            helicopter_p2_P.HILInitialize_analog_output_cha,
            num_final_analog_outputs,
            &helicopter_p2_DW.HILInitialize_AOVoltages[0]);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (num_final_pwm_outputs > 0) {
          local_result = hil_write_pwm(helicopter_p2_DW.HILInitialize_Card,
            helicopter_p2_P.HILInitialize_pwm_channels, num_final_pwm_outputs,
            &helicopter_p2_DW.HILInitialize_POValues[0]);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (result < 0) {
          msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
            (_rt_error_message));
          rtmSetErrorStatus(helicopter_p2_M, _rt_error_message);
        }
      }
    }

    hil_task_delete_all(helicopter_p2_DW.HILInitialize_Card);
    hil_monitor_delete_all(helicopter_p2_DW.HILInitialize_Card);
    hil_close(helicopter_p2_DW.HILInitialize_Card);
    helicopter_p2_DW.HILInitialize_Card = NULL;
  }

  /* Terminate for ToFile: '<Root>/To File3' */
  {
    FILE *fp = (FILE *) helicopter_p2_DW.ToFile3_PWORK.FilePtr;
    if (fp != (NULL)) {
      char fileName[509] = "p_ref_practical.mat";
      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_p2_M,
                          "Error closing MAT-file p_ref_practical.mat");
        return;
      }

      if ((fp = fopen(fileName, "r+b")) == (NULL)) {
        rtmSetErrorStatus(helicopter_p2_M,
                          "Error reopening MAT-file p_ref_practical.mat");
        return;
      }

      if (rt_WriteMat4FileHeader(fp, 2, helicopter_p2_DW.ToFile3_IWORK.Count,
           "p_ref")) {
        rtmSetErrorStatus(helicopter_p2_M,
                          "Error writing header for p_ref to MAT-file p_ref_practical.mat");
      }

      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_p2_M,
                          "Error closing MAT-file p_ref_practical.mat");
        return;
      }

      helicopter_p2_DW.ToFile3_PWORK.FilePtr = (NULL);
    }
  }

  /* Terminate for ToFile: '<S7>/To File' */
  {
    FILE *fp = (FILE *) helicopter_p2_DW.ToFile_PWORK.FilePtr;
    if (fp != (NULL)) {
      char fileName[509] = "p_dot_practical.mat";
      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_p2_M,
                          "Error closing MAT-file p_dot_practical.mat");
        return;
      }

      if ((fp = fopen(fileName, "r+b")) == (NULL)) {
        rtmSetErrorStatus(helicopter_p2_M,
                          "Error reopening MAT-file p_dot_practical.mat");
        return;
      }

      if (rt_WriteMat4FileHeader(fp, 2, helicopter_p2_DW.ToFile_IWORK.Count,
           "p_dot")) {
        rtmSetErrorStatus(helicopter_p2_M,
                          "Error writing header for p_dot to MAT-file p_dot_practical.mat");
      }

      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_p2_M,
                          "Error closing MAT-file p_dot_practical.mat");
        return;
      }

      helicopter_p2_DW.ToFile_PWORK.FilePtr = (NULL);
    }
  }

  /* Terminate for ToFile: '<S7>/To File1' */
  {
    FILE *fp = (FILE *) helicopter_p2_DW.ToFile1_PWORK.FilePtr;
    if (fp != (NULL)) {
      char fileName[509] = "p_practical.mat";
      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_p2_M,
                          "Error closing MAT-file p_practical.mat");
        return;
      }

      if ((fp = fopen(fileName, "r+b")) == (NULL)) {
        rtmSetErrorStatus(helicopter_p2_M,
                          "Error reopening MAT-file p_practical.mat");
        return;
      }

      if (rt_WriteMat4FileHeader(fp, 2, helicopter_p2_DW.ToFile1_IWORK.Count,
           "p")) {
        rtmSetErrorStatus(helicopter_p2_M,
                          "Error writing header for p to MAT-file p_practical.mat");
      }

      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_p2_M,
                          "Error closing MAT-file p_practical.mat");
        return;
      }

      helicopter_p2_DW.ToFile1_PWORK.FilePtr = (NULL);
    }
  }

  /* Terminate for ToFile: '<S7>/To File2' */
  {
    FILE *fp = (FILE *) helicopter_p2_DW.ToFile2_PWORK.FilePtr;
    if (fp != (NULL)) {
      char fileName[509] = "lambda_practical.mat";
      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_p2_M,
                          "Error closing MAT-file lambda_practical.mat");
        return;
      }

      if ((fp = fopen(fileName, "r+b")) == (NULL)) {
        rtmSetErrorStatus(helicopter_p2_M,
                          "Error reopening MAT-file lambda_practical.mat");
        return;
      }

      if (rt_WriteMat4FileHeader(fp, 2, helicopter_p2_DW.ToFile2_IWORK.Count,
           "lambda")) {
        rtmSetErrorStatus(helicopter_p2_M,
                          "Error writing header for lambda to MAT-file lambda_practical.mat");
      }

      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_p2_M,
                          "Error closing MAT-file lambda_practical.mat");
        return;
      }

      helicopter_p2_DW.ToFile2_PWORK.FilePtr = (NULL);
    }
  }

  /* Terminate for ToFile: '<S7>/To File4' */
  {
    FILE *fp = (FILE *) helicopter_p2_DW.ToFile4_PWORK.FilePtr;
    if (fp != (NULL)) {
      char fileName[509] = "r_practical.mat";
      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_p2_M,
                          "Error closing MAT-file r_practical.mat");
        return;
      }

      if ((fp = fopen(fileName, "r+b")) == (NULL)) {
        rtmSetErrorStatus(helicopter_p2_M,
                          "Error reopening MAT-file r_practical.mat");
        return;
      }

      if (rt_WriteMat4FileHeader(fp, 2, helicopter_p2_DW.ToFile4_IWORK.Count,
           "r")) {
        rtmSetErrorStatus(helicopter_p2_M,
                          "Error writing header for r to MAT-file r_practical.mat");
      }

      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_p2_M,
                          "Error closing MAT-file r_practical.mat");
        return;
      }

      helicopter_p2_DW.ToFile4_PWORK.FilePtr = (NULL);
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
  helicopter_p2_output();
  UNUSED_PARAMETER(tid);
}

void MdlUpdate(int_T tid)
{
  helicopter_p2_update();
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
  helicopter_p2_initialize();
}

void MdlTerminate(void)
{
  helicopter_p2_terminate();
}

/* Registration function */
RT_MODEL_helicopter_p2_T *helicopter_p2(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* non-finite (run-time) assignments */
  helicopter_p2_P.Integrator_UpperSat = rtInf;
  helicopter_p2_P.Integrator_LowerSat = rtMinusInf;

  /* initialize real-time model */
  (void) memset((void *)helicopter_p2_M, 0,
                sizeof(RT_MODEL_helicopter_p2_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&helicopter_p2_M->solverInfo,
                          &helicopter_p2_M->Timing.simTimeStep);
    rtsiSetTPtr(&helicopter_p2_M->solverInfo, &rtmGetTPtr(helicopter_p2_M));
    rtsiSetStepSizePtr(&helicopter_p2_M->solverInfo,
                       &helicopter_p2_M->Timing.stepSize0);
    rtsiSetdXPtr(&helicopter_p2_M->solverInfo,
                 &helicopter_p2_M->ModelData.derivs);
    rtsiSetContStatesPtr(&helicopter_p2_M->solverInfo, (real_T **)
                         &helicopter_p2_M->ModelData.contStates);
    rtsiSetNumContStatesPtr(&helicopter_p2_M->solverInfo,
      &helicopter_p2_M->Sizes.numContStates);
    rtsiSetErrorStatusPtr(&helicopter_p2_M->solverInfo, (&rtmGetErrorStatus
      (helicopter_p2_M)));
    rtsiSetRTModelPtr(&helicopter_p2_M->solverInfo, helicopter_p2_M);
  }

  rtsiSetSimTimeStep(&helicopter_p2_M->solverInfo, MAJOR_TIME_STEP);
  helicopter_p2_M->ModelData.intgData.f[0] = helicopter_p2_M->ModelData.odeF[0];
  helicopter_p2_M->ModelData.contStates = ((real_T *) &helicopter_p2_X);
  rtsiSetSolverData(&helicopter_p2_M->solverInfo, (void *)
                    &helicopter_p2_M->ModelData.intgData);
  rtsiSetSolverName(&helicopter_p2_M->solverInfo,"ode1");

  /* Initialize timing info */
  {
    int_T *mdlTsMap = helicopter_p2_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    mdlTsMap[1] = 1;
    helicopter_p2_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    helicopter_p2_M->Timing.sampleTimes =
      (&helicopter_p2_M->Timing.sampleTimesArray[0]);
    helicopter_p2_M->Timing.offsetTimes =
      (&helicopter_p2_M->Timing.offsetTimesArray[0]);

    /* task periods */
    helicopter_p2_M->Timing.sampleTimes[0] = (0.0);
    helicopter_p2_M->Timing.sampleTimes[1] = (0.002);

    /* task offsets */
    helicopter_p2_M->Timing.offsetTimes[0] = (0.0);
    helicopter_p2_M->Timing.offsetTimes[1] = (0.0);
  }

  rtmSetTPtr(helicopter_p2_M, &helicopter_p2_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits = helicopter_p2_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    mdlSampleHits[1] = 1;
    helicopter_p2_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(helicopter_p2_M, 35.0);
  helicopter_p2_M->Timing.stepSize0 = 0.002;
  helicopter_p2_M->Timing.stepSize1 = 0.002;

  /* External mode info */
  helicopter_p2_M->Sizes.checksums[0] = (1022921964U);
  helicopter_p2_M->Sizes.checksums[1] = (3318162275U);
  helicopter_p2_M->Sizes.checksums[2] = (2660293435U);
  helicopter_p2_M->Sizes.checksums[3] = (2552209957U);

  {
    static const sysRanDType rtAlwaysEnabled = SUBSYS_RAN_BC_ENABLE;
    static RTWExtModeInfo rt_ExtModeInfo;
    static const sysRanDType *systemRan[1];
    helicopter_p2_M->extModeInfo = (&rt_ExtModeInfo);
    rteiSetSubSystemActiveVectorAddresses(&rt_ExtModeInfo, systemRan);
    systemRan[0] = &rtAlwaysEnabled;
    rteiSetModelMappingInfoPtr(helicopter_p2_M->extModeInfo,
      &helicopter_p2_M->SpecialInfo.mappingInfo);
    rteiSetChecksumsPtr(helicopter_p2_M->extModeInfo,
                        helicopter_p2_M->Sizes.checksums);
    rteiSetTPtr(helicopter_p2_M->extModeInfo, rtmGetTPtr(helicopter_p2_M));
  }

  helicopter_p2_M->solverInfoPtr = (&helicopter_p2_M->solverInfo);
  helicopter_p2_M->Timing.stepSize = (0.002);
  rtsiSetFixedStepSize(&helicopter_p2_M->solverInfo, 0.002);
  rtsiSetSolverMode(&helicopter_p2_M->solverInfo, SOLVER_MODE_SINGLETASKING);

  /* block I/O */
  helicopter_p2_M->ModelData.blockIO = ((void *) &helicopter_p2_B);

  {
    int_T i;
    for (i = 0; i < 6; i++) {
      helicopter_p2_B.Gain1[i] = 0.0;
    }

    helicopter_p2_B.ElevationCounttorad = 0.0;
    helicopter_p2_B.Gain = 0.0;
    helicopter_p2_B.Sum = 0.0;
    helicopter_p2_B.PitchCounttorad = 0.0;
    helicopter_p2_B.Gain_i = 0.0;
    helicopter_p2_B.Outputfromsimulation = 0.0;
    helicopter_p2_B.TravelCounttorad = 0.0;
    helicopter_p2_B.Gain_p = 0.0;
    helicopter_p2_B.Sum3 = 0.0;
    helicopter_p2_B.Gain_d = 0.0;
    helicopter_p2_B.Gain_b = 0.0;
    helicopter_p2_B.Gain_dg = 0.0;
    helicopter_p2_B.Sum1 = 0.0;
    helicopter_p2_B.Gain1_f = 0.0;
    helicopter_p2_B.Sum2 = 0.0;
    helicopter_p2_B.K_ei = 0.0;
    helicopter_p2_B.Gain_l = 0.0;
    helicopter_p2_B.BackmotorSaturation = 0.0;
    helicopter_p2_B.FrontmotorSaturation = 0.0;
  }

  /* parameters */
  helicopter_p2_M->ModelData.defaultParam = ((real_T *)&helicopter_p2_P);

  /* states (continuous) */
  {
    real_T *x = (real_T *) &helicopter_p2_X;
    helicopter_p2_M->ModelData.contStates = (x);
    (void) memset((void *)&helicopter_p2_X, 0,
                  sizeof(X_helicopter_p2_T));
  }

  /* states (dwork) */
  helicopter_p2_M->ModelData.dwork = ((void *) &helicopter_p2_DW);
  (void) memset((void *)&helicopter_p2_DW, 0,
                sizeof(DW_helicopter_p2_T));

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      helicopter_p2_DW.HILInitialize_AIMinimums[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      helicopter_p2_DW.HILInitialize_AIMaximums[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      helicopter_p2_DW.HILInitialize_AOMinimums[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      helicopter_p2_DW.HILInitialize_AOMaximums[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      helicopter_p2_DW.HILInitialize_AOVoltages[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      helicopter_p2_DW.HILInitialize_FilterFrequency[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      helicopter_p2_DW.HILInitialize_POSortedFreqs[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      helicopter_p2_DW.HILInitialize_POValues[i] = 0.0;
    }
  }

  helicopter_p2_DW.TimeStampA = 0.0;
  helicopter_p2_DW.LastUAtTimeA = 0.0;
  helicopter_p2_DW.TimeStampB = 0.0;
  helicopter_p2_DW.LastUAtTimeB = 0.0;
  helicopter_p2_DW.HILWriteAnalog_Buffer[0] = 0.0;
  helicopter_p2_DW.HILWriteAnalog_Buffer[1] = 0.0;

  /* data type transition information */
  {
    static DataTypeTransInfo dtInfo;
    (void) memset((char_T *) &dtInfo, 0,
                  sizeof(dtInfo));
    helicopter_p2_M->SpecialInfo.mappingInfo = (&dtInfo);
    dtInfo.numDataTypes = 16;
    dtInfo.dataTypeSizes = &rtDataTypeSizes[0];
    dtInfo.dataTypeNames = &rtDataTypeNames[0];

    /* Block I/O transition table */
    dtInfo.B = &rtBTransTable;

    /* Parameters transition table */
    dtInfo.P = &rtPTransTable;
  }

  /* Initialize Sizes */
  helicopter_p2_M->Sizes.numContStates = (4);/* Number of continuous states */
  helicopter_p2_M->Sizes.numY = (0);   /* Number of model outputs */
  helicopter_p2_M->Sizes.numU = (0);   /* Number of model inputs */
  helicopter_p2_M->Sizes.sysDirFeedThru = (0);/* The model is not direct feedthrough */
  helicopter_p2_M->Sizes.numSampTimes = (2);/* Number of sample times */
  helicopter_p2_M->Sizes.numBlocks = (66);/* Number of blocks */
  helicopter_p2_M->Sizes.numBlockIO = (20);/* Number of block outputs */
  helicopter_p2_M->Sizes.numBlockPrms = (143);/* Sum of parameter "widths" */
  return helicopter_p2_M;
}

/*========================================================================*
 * End of Classic call interface                                          *
 *========================================================================*/
