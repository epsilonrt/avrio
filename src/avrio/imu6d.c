/**
 * @file imu6d.c
 * @brief Inertial Measurement Unit Filter (Implémentation)
 * @author Copyright © 2012 epsilonRT. All rights reserved.
 * @author Aaron Berk © 2010 ARM Limited. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *

 * Revision History ------------------------------------------------------------
 *    20121225 - Initial version by epsilonRT
 */

#include <math.h>
#include "imu6d.h"

/* internal public functions ================================================ */
// -----------------------------------------------------------------------------
void 
vImu6dReset(xImu6dFilter * xFilter) {

  xFilter->bFirstUpdate = false;

  //Quatfernion orientation of earth frame relative to auxiliary frame.
  xFilter->xAEq.a = 1;
  xFilter->xAEq.b = 0;
  xFilter->xAEq.c = 0;
  xFilter->xAEq.d = 0;

  //Estimated orientation quaternionf elements with initial conditions.
  xFilter->xSEq.a = 1;
  xFilter->xSEq.b = 0;
  xFilter->xSEq.c = 0;
  xFilter->xSEq.d = 0;
}

// -----------------------------------------------------------------------------
void 
vImu6dInit (xImu6dFilter * xFilter, float fFilterRate, float fGyroError){

  vImu6dReset(xFilter);

  // Sampling period (typical value is ~0.1s).
  xFilter->fDeltaT = fFilterRate;
  
  // Gyroscope measurement error (in degrees per second).
  xFilter->fGyroMeasError = fGyroError;
  
  // Compute xFilter->fBeta.
  xFilter->fBeta = sqrt(3.0 / 4.0) * (M_PI * (xFilter->fGyroMeasError / 180.0));
}

// -----------------------------------------------------------------------------
void 
vImu6dUpdateFilter(xImu6dFilter * xFilter, float * xAcc, float * xRate) {

  // Local system variables.

  // Quatfernion rate from gyroscope elements.
  xQuatf xSEqDotOmega;
  
  // Objective function elements.
  float f_1;
  float f_2;
  float f_3;
  
  // Objective function Jacobian elements.
  float J_11or24;
  float J_12or23;
  float J_13or22;
  float J_14or21;
  float J_32;
  float J_33;
  
  // Objective function gradient elements.
  xQuatf xNablaF;

  // Auxiliary variables to avoid reapeated calcualtions.
  float halfSEq_1 = 0.5 * xFilter->xSEq.a;
  float halfSEq_2 = 0.5 * xFilter->xSEq.b;
  float halfSEq_3 = 0.5 * xFilter->xSEq.c;
  float halfSEq_4 = 0.5 * xFilter->xSEq.d;
  
  float twoSEq_1 = 2.0 * xFilter->xSEq.a;
  float twoSEq_2 = 2.0 * xFilter->xSEq.b;
  float twoSEq_3 = 2.0 * xFilter->xSEq.c;

  // Compute the quaternionf rate measured by gyroscopes.
  xSEqDotOmega.a = -halfSEq_2 * xRate[0] - halfSEq_3 * xRate[1] - 
                    halfSEq_4 * xRate[2];
  xSEqDotOmega.b =  halfSEq_1 * xRate[0] + halfSEq_3 * xRate[2] - 
                    halfSEq_4 * xRate[1];
  xSEqDotOmega.c =  halfSEq_1 * xRate[1] - halfSEq_2 * xRate[2] + 
                    halfSEq_4 * xRate[0];
  xSEqDotOmega.d =  halfSEq_1 * xRate[2] + halfSEq_2 * xRate[1] - 
                    halfSEq_3 * xRate[0];

  // Normalise the accelerometer measurement.
  vVector3fNormalize (xAcc);

  // Compute the objective function and Jacobian.
  f_1 = twoSEq_2 * xFilter->xSEq.d - twoSEq_1 * xFilter->xSEq.c - xAcc[0];
  f_2 = twoSEq_1 * xFilter->xSEq.b + twoSEq_3 * xFilter->xSEq.d - xAcc[1];
  f_3 = 1.0 - twoSEq_2 * xFilter->xSEq.b - twoSEq_3 * 
        xFilter->xSEq.c - xAcc[2];
  
  // J_11 negated in matrix multiplication.
  J_11or24 = twoSEq_3;
  J_12or23 = 2 * xFilter->xSEq.d;
  
  // J_12 negated in matrix multiplication
  J_13or22 = twoSEq_1;
  J_14or21 = twoSEq_2;
  
  // Negated in matrix multiplication.
  J_32 = 2 * J_14or21;
  
  // Negated in matrix multiplication.
  J_33 = 2 * J_11or24;

  //Compute the gradient (matrix multiplication).
  xNablaF.a = J_14or21 * f_2 - J_11or24 * f_1;
  xNablaF.b = J_12or23 * f_1 + J_13or22 * f_2 - J_32 * f_3;
  xNablaF.c = J_12or23 * f_2 - J_33 * f_3 - J_13or22 * f_1;
  xNablaF.d = J_14or21 * f_1 + J_11or24 * f_2;

  // Normalise the gradient.
  vQuatfNormalize (&xNablaF);

  // Compute then integrate the estimated quaternionf rate.
  xFilter->xSEq.a += (xSEqDotOmega.a - (xFilter->fBeta * xNablaF.a)) * 
                      xFilter->fDeltaT;
  xFilter->xSEq.b += (xSEqDotOmega.b - (xFilter->fBeta * xNablaF.b)) * 
                      xFilter->fDeltaT;
  xFilter->xSEq.c += (xSEqDotOmega.c - (xFilter->fBeta * xNablaF.c)) * 
                      xFilter->fDeltaT;
  xFilter->xSEq.d += (xSEqDotOmega.d - (xFilter->fBeta * xNablaF.d)) * 
                      xFilter->fDeltaT;

  // Normalise quaternionf
  vQuatfNormalize (&xFilter->xSEq);

  if (xFilter->bFirstUpdate == false) {
  
      // Store orientation of auxiliary frame.
      vQuatfCopy (&xFilter->xAEq, &xFilter->xSEq);
      xFilter->bFirstUpdate = true;
  }
}

// -----------------------------------------------------------------------------
void 
vImu6dComputeEuler (xImu6dFilter * xFilter) {

  // Quatfernion describing orientation of sensor relative to earth.
  xQuatf xESq;
  
  // Quatfernion describing orientation of sensor relative to auxiliary frame.
  xQuatf xASq;
                            
  // Compute the quaternionf conjugate.
  xESq.a =  xFilter->xSEq.a;
  xESq.b = -xFilter->xSEq.b;
  xESq.c = -xFilter->xSEq.c;
  xESq.d = -xFilter->xSEq.d;

  // Compute the quaternionf product.
  xASq.a = xESq.a * xFilter->xAEq.a - xESq.b * xFilter->xAEq.b - 
           xESq.c * xFilter->xAEq.c - xESq.d * xFilter->xAEq.d;
  xASq.b = xESq.a * xFilter->xAEq.b + xESq.b * xFilter->xAEq.a + 
           xESq.c * xFilter->xAEq.d - xESq.d * xFilter->xAEq.c;
  xASq.c = xESq.a * xFilter->xAEq.c - xESq.b * xFilter->xAEq.d + 
           xESq.c * xFilter->xAEq.a + xESq.d * xFilter->xAEq.b;
  xASq.d = xESq.a * xFilter->xAEq.d + xESq.b * xFilter->xAEq.c - 
           xESq.c * xFilter->xAEq.b + xESq.d * xFilter->xAEq.a;

  // Compute the Euler angles from the quaternionf.
  xFilter->xAtt[0]  = atan2( 2 * xASq.c * xASq.d - 2 * xASq.a * xASq.b, 
                               2 * xASq.a * xASq.a + 2 * xASq.d * xASq.d - 1);
  xFilter->xAtt[1] = asin ( 2 * xASq.b * xASq.c - 2 * xASq.a * xASq.c);
  xFilter->xAtt[2]   = atan2( 2 * xASq.b * xASq.c - 2 * xASq.a * xASq.d, 
                               2 * xASq.a * xASq.a + 2 * xASq.b * xASq.b - 1);
}

/* ========================================================================== */
