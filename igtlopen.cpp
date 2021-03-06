/*=========================================================================

  Program:   Matlab Open IGT Link Interface -- igtlsend
  Module:    $RCSfile: $
  Language:  C++
  Date:      $Date: $
  Version:   $Revision: $

  Copyright (c) Insight Software Consortium. All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

//
//  SD = igtlopen(HOST, PORT);
//
//    SD  : (integer)    Socket descriptor (-1 if failed to connect)
//    HOST: (string)     Hostname or IP address. If '' is specified,
//                       server socket will be created.
//    PORT: (int)        Port #
//

#include "mex.h"
#include <math.h>
#include <string.h>

#include "igtlOSUtil.h"
#include "igtlImageMessage.h"
#include "igtlMexClientSocket.h"
#include "igtlMexServerSocket.h"

using namespace std;

//#define pi (3.141592653589793)

#define ARG_ID_HOST    0  // host
#define ARG_ID_PORT    1  // port
#define ARG_ID_NUM     2  // total number of arguments

#define MAX_STRING_LEN 256

//extern void _main();

// -----------------------------------------------------------------
// Function declarations.
double& createMatlabScalar (mxArray*& ptr);

// -----------------------------------------------------------------
// Function definitions.
void mexFunction (int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray *prhs[]) {
  
  // ---------------------------------------------------------------
  // Check numbers of arguments and outputs
  if (nrhs != ARG_ID_NUM)
    {
    mexErrMsgTxt("Incorrect number of input arguments");
    }

  if (nlhs != 1)
    {
    mexErrMsgTxt("Incorrect number of output arguments");
    }

  // ---------------------------------------------------------------
  // Check types of arguments

  // HOST
  if (!mxIsChar(prhs[ARG_ID_HOST]))
    {
    mexErrMsgTxt("HOST argument must be string.");
    }

  // PORT
  if (!mxIsNumeric(prhs[ARG_ID_PORT]))
    {
    mexErrMsgTxt("PORT argument must be integer.");
    }

  // ---------------------------------------------------------------
  // Set variables

  char hostname[MAX_STRING_LEN+1];
  
  mxGetString(prhs[ARG_ID_HOST], hostname, MAX_STRING_LEN);
  int         port     = (int)*mxGetPr(prhs[ARG_ID_PORT]);

  // TODO: check length of hostname



  // ---------------------------------------------------------------
  // Server or Client?

  double& p = createMatlabScalar(plhs[0]);

  if (strlen(hostname) == 0)  // Server
    {
    // ----------------------------------------
    // Set up OpenIGTLink server socket
    igtl::MexServerSocket::Pointer serverSocket;
    serverSocket = igtl::MexServerSocket::New();
    int r = serverSocket->CreateServer(port);

    if (r < 0)
      {
      p = -1;
      mexErrMsgTxt("Cannot create a server socket.");
      }
    else
      {
      p = serverSocket->GetDescriptor();
      }
    }
  else // Client
    {
    // ----------------------------------------
    // Set up OpenIGTLink client socket
    igtl::MexClientSocket::Pointer socket;
    socket = igtl::MexClientSocket::New();
    
    int result = socket->ConnectToServer(hostname, port);
    //sleep(2);
    if (result != 0)
      {
      p = -1;
      }
    else
      {
      p = socket->GetDescriptor();
      }
    }

}

double& createMatlabScalar (mxArray*& ptr) { 
  ptr = mxCreateDoubleMatrix(1,1,mxREAL);
  return *mxGetPr(ptr);
}



