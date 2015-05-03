/**
 * Copyright © 2011-2015 Pascal JEAN aka epsilonRT. All rights reserved.
 *
 * This file is part of AvrIO.
 *
 * AvrIO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * AvrIO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with AvrIO.  If not, see <http://www.gnu.org/licenses/lgpl.html>
 *
 * @file kalman.c
 * @brief Filtre de Kalman (Implémentation)
 * Copyright © 2012 Kristian Lauszus, TKJ Electronics. All rights reserved.
 *
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

#include "kalman.h"

/* internal public functions ================================================ */
// -----------------------------------------------------------------------------
void vKalmanInit (xKalmanFilter * xFilter) {

  /* We will set the variables like so, these can also be tuned by the user */
  xFilter->fQAngle = KALMAN_Q_ANGLE;
  xFilter->fQBias = KALMAN_Q_BIAS;
  xFilter->fRMeasure = KALMAN_R_MEASURE;

  xFilter->fBias = 0; // Reset bias
  /* 
    Since we assume tha the bias is 0 and we know the starting angle 
    (use setAngle), the error covariance matrix is set like so 
    see: http://en.wikipedia.org/wiki/Kalman_filter#Example_application.2C_technical
   */
  xFilter->fP[0][0] = 0; 
  xFilter->fP[0][1] = 0;
  xFilter->fP[1][0] = 0;
  xFilter->fP[1][1] = 0;
};

// -----------------------------------------------------------------------------
void 
vKalmanProcess (xKalmanFilter * xFilter, 
             double fNewAngle, double fNewRate, uint16_t usDt) {
  double fDt = (double)usDt / 1000.0;
  // KasBot V2  -  Kalman filter module - http://www.x-firm.com/?page_id=145
  // Modified by Kristian Lauszus
  // See my blog post for more information: 
  // http://blog.tkjelectronics.dk/2012/09/a-practical-approach-to-kalman-filter-and-how-to-implement-it
  // Discrete Kalman filter time update equations - Time Update ("Predict")
  // Update xhat - Project the state ahead

  /* Step 1 */
  xFilter->fRate = fNewRate - xFilter->fBias;
  xFilter->fAngle += fDt * xFilter->fRate;
  
  /* Step 2 */
  // Update estimation error covariance - Project the error covariance ahead
  xFilter->fP[0][0] += fDt * (fDt * xFilter->fP[1][1] - xFilter->fP[0][1] -
                             xFilter->fP[1][0] + xFilter->fQAngle);
  xFilter->fP[0][1] -= fDt * xFilter->fP[1][1];
  xFilter->fP[1][0] -= fDt * xFilter->fP[1][1];
  xFilter->fP[1][1] += xFilter->fQBias * fDt;
  
  /* Step 4 */
  // Discrete Kalman filter measurement update equations - Measurement Update ("Correct")
  // Calculate Kalman gain - Compute the Kalman gain
  xFilter->fS = xFilter->fP[0][0] + xFilter->fRMeasure;

  /* Step 5 */
  xFilter->fK[0] = xFilter->fP[0][0] / xFilter->fS;
  xFilter->fK[1] = xFilter->fP[1][0] / xFilter->fS;
  
  /* Step 3 */
  // Calculate angle and bias - Update estimate with measurement zk (fNewAngle)
  xFilter->fY = fNewAngle - xFilter->fAngle;
  
  /* Step 6 */
  xFilter->fAngle += xFilter->fK[0] * xFilter->fY;
  xFilter->fBias += xFilter->fK[1] * xFilter->fY;
  
  /* Step 7 */
  // Calculate estimation error covariance - Update the error covariance
  xFilter->fP[0][0] -= xFilter->fK[0] * xFilter->fP[0][0];
  xFilter->fP[0][1] -= xFilter->fK[0] * xFilter->fP[0][1];
  xFilter->fP[1][0] -= xFilter->fK[1] * xFilter->fP[0][0];
  xFilter->fP[1][1] -= xFilter->fK[1] * xFilter->fP[0][1];
}

/* ========================================================================== */
