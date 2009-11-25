/*
 * Copyright (c) 2005 The University of Notre Dame. All Rights Reserved.
 *
 * The University of Notre Dame grants you ("Licensee") a
 * non-exclusive, royalty free, license to use, modify and
 * redistribute this software in source and binary code form, provided
 * that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 * This software is provided "AS IS," without a warranty of any
 * kind. All express or implied conditions, representations and
 * warranties, including any implied warranty of merchantability,
 * fitness for a particular purpose or non-infringement, are hereby
 * excluded.  The University of Notre Dame and its licensors shall not
 * be liable for any damages suffered by licensee as a result of
 * using, modifying or distributing the software or its
 * derivatives. In no event will the University of Notre Dame or its
 * licensors be liable for any lost revenue, profit or data, or for
 * direct, indirect, special, consequential, incidental or punitive
 * damages, however caused and regardless of the theory of liability,
 * arising out of the use of or inability to use software, even if the
 * University of Notre Dame has been advised of the possibility of
 * such damages.
 *
 * SUPPORT OPEN SCIENCE!  If you use OpenMD or its source code in your
 * research, please cite the appropriate papers when you publish your
 * work.  Good starting points are:
 *                                                                      
 * [1]  Meineke, et al., J. Comp. Chem. 26, 252-271 (2005).             
 * [2]  Fennell & Gezelter, J. Chem. Phys. 124, 234104 (2006).          
 * [3]  Sun, Lin & Gezelter, J. Chem. Phys. 128, 24107 (2008).          
 * [4]  Vardeman & Gezelter, in progress (2009).                        
 */
 
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "math/MatVec3.h"

/* 
 * Contains various utilities for dealing with 3x3 matrices and 
 * length 3 vectors
 */

void identityMat3(RealType A[3][3]) {
  int i;
  for (i = 0; i < 3; i++) {
    A[i][0] = A[i][1] = A[i][2] = 0.0;
    A[i][i] = 1.0;
  }
}

void swapVectors3(RealType v1[3], RealType v2[3]) {
  int i;
  for (i = 0; i < 3; i++) {
    RealType tmp = v1[i];
    v1[i] = v2[i];
    v2[i] = tmp;
  }
}

RealType normalize3(RealType x[3]) {
  RealType den; 
  int i;
  if ( (den = norm3(x)) != 0.0 ) {
    for (i=0; i < 3; i++)
      {
        x[i] /= den;
      }
  }
  return den;
}

void matMul3(RealType a[3][3], RealType b[3][3], RealType c[3][3]) {
  RealType r00, r01, r02, r10, r11, r12, r20, r21, r22;
  
  r00 = a[0][0]*b[0][0] + a[0][1]*b[1][0] + a[0][2]*b[2][0];
  r01 = a[0][0]*b[0][1] + a[0][1]*b[1][1] + a[0][2]*b[2][1];
  r02 = a[0][0]*b[0][2] + a[0][1]*b[1][2] + a[0][2]*b[2][2];
  
  r10 = a[1][0]*b[0][0] + a[1][1]*b[1][0] + a[1][2]*b[2][0];
  r11 = a[1][0]*b[0][1] + a[1][1]*b[1][1] + a[1][2]*b[2][1];
  r12 = a[1][0]*b[0][2] + a[1][1]*b[1][2] + a[1][2]*b[2][2];
  
  r20 = a[2][0]*b[0][0] + a[2][1]*b[1][0] + a[2][2]*b[2][0];
  r21 = a[2][0]*b[0][1] + a[2][1]*b[1][1] + a[2][2]*b[2][1];
  r22 = a[2][0]*b[0][2] + a[2][1]*b[1][2] + a[2][2]*b[2][2];
  
  c[0][0] = r00; c[0][1] = r01; c[0][2] = r02;
  c[1][0] = r10; c[1][1] = r11; c[1][2] = r12;
  c[2][0] = r20; c[2][1] = r21; c[2][2] = r22;
}

void matVecMul3(RealType m[3][3], RealType inVec[3], RealType outVec[3]) {
  RealType a0, a1, a2;
  
  a0 = inVec[0];  a1 = inVec[1];  a2 = inVec[2];
  
  outVec[0] = m[0][0]*a0 + m[0][1]*a1 + m[0][2]*a2;
  outVec[1] = m[1][0]*a0 + m[1][1]*a1 + m[1][2]*a2;
  outVec[2] = m[2][0]*a0 + m[2][1]*a1 + m[2][2]*a2;
}

RealType matDet3(RealType a[3][3]) {
  int i, j, k;
  RealType determinant;
  
  determinant = 0.0;
  
  for(i = 0; i < 3; i++) {
    j = (i+1)%3;
    k = (i+2)%3;
    
    determinant += a[0][i] * (a[1][j]*a[2][k] - a[1][k]*a[2][j]);
  }
  
  return determinant;
}

void invertMat3(RealType a[3][3], RealType b[3][3]) {
  
  int  i, j, k, l, m, n;
  RealType determinant;
  
  determinant = matDet3( a );
  
  if (determinant == 0.0) {
    sprintf( painCave.errMsg,
             "Can't invert a matrix with a zero determinant!\n");
    painCave.isFatal = 1;
    simError();
  }
  
  for (i=0; i < 3; i++) {
    j = (i+1)%3;
    k = (i+2)%3;
    for(l = 0; l < 3; l++) {
      m = (l+1)%3;
      n = (l+2)%3;
      
      b[l][i] = (a[j][m]*a[k][n] - a[j][n]*a[k][m]) / determinant;
    }
  }
}

void transposeMat3(RealType in[3][3], RealType out[3][3]) {
  RealType temp[3][3];
  int i, j;
  
  for (i = 0; i < 3; i++) {
    for (j = 0; j < 3; j++) {
      temp[j][i] = in[i][j];
    }
  }
  for (i = 0; i < 3; i++) {
    for (j = 0; j < 3; j++) {
      out[i][j] = temp[i][j];
    }
  }
}

void printMat3(RealType A[3][3] ){
  
  fprintf(stderr, "[ %g, %g, %g ]\n[ %g, %g, %g ]\n[ %g, %g, %g ]\n",          
          A[0][0] , A[0][1] , A[0][2], 
          A[1][0] , A[1][1] , A[1][2], 
          A[2][0] , A[2][1] , A[2][2]) ;
}

void printMat9(RealType A[9] ){
  
  fprintf(stderr, "[ %g, %g, %g ]\n[ %g, %g, %g ]\n[ %g, %g, %g ]\n",          
          A[0], A[1], A[2],
          A[3], A[4], A[5],
          A[6], A[7], A[8]);
}

RealType matTrace3(RealType m[3][3]){
  RealType trace;
  trace = m[0][0] + m[1][1] + m[2][2];
  
  return trace;
}

void crossProduct3(RealType a[3],RealType b[3], RealType out[3]){
  
  out[0] = a[1] * b[2] - a[2] * b[1];
  out[1] = a[2] * b[0] - a[0] * b[2] ;
  out[2] = a[0] * b[1] - a[1] * b[0];
  
}

RealType dotProduct3(RealType a[3], RealType b[3]){
  return a[0]*b[0] + a[1]*b[1]+ a[2]*b[2];
}

/*----------------------------------------------------------------------------*/
/* Extract the eigenvalues and eigenvectors from a 3x3 matrix.*/
/* The eigenvectors (the columns of V) will be normalized. */
/* The eigenvectors are aligned optimally with the x, y, and z*/
/* axes respectively.*/

void diagonalize3x3(const RealType A[3][3], RealType w[3], RealType V[3][3]) {
  int i,j,k,maxI;
  RealType tmp, maxVal;

  /* do the matrix[3][3] to **matrix conversion for Jacobi*/
  RealType C[3][3];
  RealType *ATemp[3],*VTemp[3];
  for (i = 0; i < 3; i++)
    {
      C[i][0] = A[i][0];
      C[i][1] = A[i][1];
      C[i][2] = A[i][2];
      ATemp[i] = C[i];
      VTemp[i] = V[i];
    }
  
  /* diagonalize using Jacobi*/
  JacobiN(ATemp,3,w,VTemp);
  
  /* if all the eigenvalues are the same, return identity matrix*/
  if (w[0] == w[1] && w[0] == w[2])
    {
      identityMat3(V);
      return;
    }
  
  /* transpose temporarily, it makes it easier to sort the eigenvectors*/
  transposeMat3(V,V);
  
  /* if two eigenvalues are the same, re-orthogonalize to optimally line*/
  /* up the eigenvectors with the x, y, and z axes*/
  for (i = 0; i < 3; i++)
    {
      if (w[(i+1)%3] == w[(i+2)%3]) /* two eigenvalues are the same*/
        {
          /* find maximum element of the independant eigenvector*/
          maxVal = fabs(V[i][0]);
          maxI = 0;
          for (j = 1; j < 3; j++)
            {
              if (maxVal < (tmp = fabs(V[i][j])))
                {
                  maxVal = tmp;
                  maxI = j;
                }
            }
          /* swap the eigenvector into its proper position*/
          if (maxI != i)
            {
              tmp = w[maxI];
              w[maxI] = w[i];
              w[i] = tmp;
              swapVectors3(V[i],V[maxI]);
            }
          /* maximum element of eigenvector should be positive*/
          if (V[maxI][maxI] < 0)
            {
              V[maxI][0] = -V[maxI][0];
              V[maxI][1] = -V[maxI][1];
              V[maxI][2] = -V[maxI][2];
            }
          
          /* re-orthogonalize the other two eigenvectors*/
          j = (maxI+1)%3;
          k = (maxI+2)%3;
          
          V[j][0] = 0.0; 
          V[j][1] = 0.0; 
          V[j][2] = 0.0;
          V[j][j] = 1.0;
          crossProduct3(V[maxI],V[j],V[k]);
          normalize3(V[k]);
          crossProduct3(V[k],V[maxI],V[j]);

          /* transpose vectors back to columns*/
          transposeMat3(V,V);
          return;
        }
    }
  
  /* the three eigenvalues are different, just sort the eigenvectors*/
  /* to align them with the x, y, and z axes*/
  
  /* find the vector with the largest x element, make that vector*/
  /* the first vector*/
  maxVal = fabs(V[0][0]);
  maxI = 0;
  for (i = 1; i < 3; i++)
    {
      if (maxVal < (tmp = fabs(V[i][0])))
        {
          maxVal = tmp;
          maxI = i;
        }
    }
  /* swap eigenvalue and eigenvector*/
  if (maxI != 0)
    {
      tmp = w[maxI];
      w[maxI] = w[0];
      w[0] = tmp;
      swapVectors3(V[maxI],V[0]);
    }
  /* do the same for the y element*/
  if (fabs(V[1][1]) < fabs(V[2][1]))
    {
      tmp = w[2];
      w[2] = w[1];
      w[1] = tmp;
      swapVectors3(V[2],V[1]);
    }
  
  /* ensure that the sign of the eigenvectors is correct*/
  for (i = 0; i < 2; i++)
    {
      if (V[i][i] < 0)
        {
          V[i][0] = -V[i][0];
          V[i][1] = -V[i][1];
          V[i][2] = -V[i][2];
        }
    }
  /* set sign of final eigenvector to ensure that determinant is positive*/
  if (matDet3(V) < 0)
    {
      V[2][0] = -V[2][0];
      V[2][1] = -V[2][1];
      V[2][2] = -V[2][2];
    }
  
  /* transpose the eigenvectors back again*/
  transposeMat3(V,V);
}


#define MAT_ROTATE(a,i,j,k,l) g=a[i][j];h=a[k][l];a[i][j]=g-s*(h+g*tau); a[k][l]=h+s*(g-h*tau);

#define MAX_ROTATIONS 20

/* Jacobi iteration for the solution of eigenvectors/eigenvalues of a nxn*/
/* real symmetric matrix. Square nxn matrix a; size of matrix in n;*/
/* output eigenvalues in w; and output eigenvectors in v. Resulting*/
/* eigenvalues/vectors are sorted in decreasing order; eigenvectors are*/
/* normalized.*/
int JacobiN(RealType **a, int n, RealType *w, RealType **v) {

  int i, j, k, iq, ip, numPos;
  int ceil_half_n;
  RealType tresh, theta, tau, t, sm, s, h, g, c, tmp;
  RealType bspace[4], zspace[4];
  RealType *b = bspace;
  RealType *z = zspace;
  

  /* only allocate memory if the matrix is large*/
  if (n > 4)
    {
      b = (RealType *) calloc(n, sizeof(RealType));
      z = (RealType *) calloc(n, sizeof(RealType));
    }
  
  /* initialize*/
  for (ip=0; ip<n; ip++) 
    {
      for (iq=0; iq<n; iq++)
        {
          v[ip][iq] = 0.0;
        }
      v[ip][ip] = 1.0;
    }
  for (ip=0; ip<n; ip++) 
    {
      b[ip] = w[ip] = a[ip][ip];
      z[ip] = 0.0;
    }
  
  /* begin rotation sequence*/
  for (i=0; i<MAX_ROTATIONS; i++) 
    {
      sm = 0.0;
      for (ip=0; ip<n-1; ip++) 
        {
          for (iq=ip+1; iq<n; iq++)
            {
              sm += fabs(a[ip][iq]);
            }
        }
      if (sm == 0.0)
        {
          break;
        }
      
      if (i < 3)                                /* first 3 sweeps*/
        {
          tresh = 0.2*sm/(n*n);
        }
      else
        {
          tresh = 0.0;
        }
      
      for (ip=0; ip<n-1; ip++) 
        {
          for (iq=ip+1; iq<n; iq++) 
            {
              g = 100.0*fabs(a[ip][iq]);
              
              /* after 4 sweeps*/
              if (i > 3 && (fabs(w[ip])+g) == fabs(w[ip])
                  && (fabs(w[iq])+g) == fabs(w[iq]))
                {
                  a[ip][iq] = 0.0;
                }
              else if (fabs(a[ip][iq]) > tresh) 
                {
                  h = w[iq] - w[ip];
                  if ( (fabs(h)+g) == fabs(h))
                    {
                      t = (a[ip][iq]) / h;
                    }
                  else 
                    {
                      theta = 0.5*h / (a[ip][iq]);
                      t = 1.0 / (fabs(theta)+sqrt(1.0+theta*theta));
                      if (theta < 0.0)
                        {
                          t = -t;
                        }
                    }
                  c = 1.0 / sqrt(1+t*t);
                  s = t*c;
                  tau = s/(1.0+c);
                  h = t*a[ip][iq];
                  z[ip] -= h;
                  z[iq] += h;
                  w[ip] -= h;
                  w[iq] += h;
                  a[ip][iq]=0.0;
                  
                  /* ip already shifted left by 1 unit*/
                  for (j = 0;j <= ip-1;j++) 
                    {
                      MAT_ROTATE(a,j,ip,j,iq)
                        }
                  /* ip and iq already shifted left by 1 unit*/
                  for (j = ip+1;j <= iq-1;j++) 
                    {
                      MAT_ROTATE(a,ip,j,j,iq)
                        }
                  /* iq already shifted left by 1 unit*/
                  for (j=iq+1; j<n; j++) 
                    {
                      MAT_ROTATE(a,ip,j,iq,j)
                        }
                  for (j=0; j<n; j++) 
                    {
                      MAT_ROTATE(v,j,ip,j,iq)
                        }
                }
            }
        }
      
      for (ip=0; ip<n; ip++) 
        {
          b[ip] += z[ip];
          w[ip] = b[ip];
          z[ip] = 0.0;
        }
    }
  
  /*// this is NEVER called*/
  if ( i >= MAX_ROTATIONS )
    {
      sprintf( painCave.errMsg,
               "Jacobi: Error extracting eigenfunctions!\n");
      painCave.isFatal = 1;
      simError();      
      return 0;
    }
  
  /* sort eigenfunctions                 these changes do not affect accuracy */
  for (j=0; j<n-1; j++)                  /* boundary incorrect*/
    {
      k = j;
      tmp = w[k];
      for (i=j+1; i<n; i++)             /* boundary incorrect, shifted already*/
        {
          if (w[i] >= tmp)                   /* why exchage if same?*/
            {
              k = i;
              tmp = w[k];
            }
        }
      if (k != j) 
        {
          w[k] = w[j];
          w[j] = tmp;
          for (i=0; i<n; i++) 
            {
              tmp = v[i][j];
              v[i][j] = v[i][k];
              v[i][k] = tmp;
            }
        }
    }
  /* insure eigenvector consistency (i.e., Jacobi can compute vectors that*/
  /* are negative of one another (.707,.707,0) and (-.707,-.707,0). This can*/
  /* reek havoc in hyperstreamline/other stuff. We will select the most*/
  /* positive eigenvector.*/
  ceil_half_n = (n >> 1) + (n & 1);
  for (j=0; j<n; j++)
    {
      for (numPos=0, i=0; i<n; i++)
        {
          if ( v[i][j] >= 0.0 )
            {
              numPos++;
            }
        }
      /*    if ( numPos < ceil(RealType(n)/RealType(2.0)) )*/
      if ( numPos < ceil_half_n)
        {
          for(i=0; i<n; i++)
            {
              v[i][j] *= -1.0;
            }
        }
    }
  
  if (n > 4)
    {
      free(b);
      free(z);
    }
  return 1;
}

#undef MAT_ROTATE
#undef MAX_ROTATIONS
