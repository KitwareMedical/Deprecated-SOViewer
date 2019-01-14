/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovPhantomRenderer.h,v $
  Language:  C++
  Date:      $Date: 2005-01-27 23:31:58 $
  Version:   $Revision: 1.1.1.1 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __sovPhantomRenderer_h
#define __sovPhantomRenderer_h

#include "sovRenderer.h"

# include <iostream>  // Include real ansi istream and ostream.
# include <strstream> // Include real ansi strstreams.
# include <fstream>   // Include real ansi ifstream and ofstream.
# include <iomanip>   // Include real ansi io manipulators.
using std::dec;
using std::hex;
using std::setw;
using std::setfill;
using std::setprecision;
using std::cerr;
using std::cout;
using std::cin;
using std::ios;
using std::endl;
using std::ends;
using std::ostream;
using std::istream;
using std::ostrstream;
using std::istrstream;
using std::strstream;
using std::ofstream;
using std::ifstream;
using std::fstream;

#include "sovPhantomInteractor.h"
#include "sovPhantomRenderMethodFactory.h"
#include "sovPhantomRenderMethod.h"

namespace sov
{

/** This class supports SensAble Phantom rendering.
 */
class PhantomRenderer
: public Renderer<3>
{

public:

  typedef PhantomRenderer       Self;
  typedef SmartPointer< Self >   Pointer;
  typedef Renderer<3>   Superclass;
  typedef Superclass::SceneType  SceneType;
  typedef PhantomRenderMethod MethodType;
  typedef Superclass::MethodListType MethodListType;

  itkNewMacro( Self );
  itkTypeMacro( PhantomRenderer, Renderer );
  
  void AddObject( ObjectType * );
  void AddObject(ObjectType *, MethodType * );
  void SetScene( SceneType * );
  void Update();
  sov::PhantomInteractor* GetPhantomInteractor() {return &m_Phantom;}
  
  /** Start the Phantom */
  void Start();
    
  /** Stop the Phantom */
  void Stop();
  
  MethodPointer GetRenderMethodViaFactory(const char * nametype, 
                                    unsigned int dimension, 
                                    const char* type,
                                    const char* methodName,
                                    const char* pixelType=NULL)
                                    {
                                    std::cout << "PhantomRenderMethodFactory" << std::endl;
                                    return PhantomRenderMethodFactory<3>::CreateRenderMethod( nametype, 
                                            3, type, methodName, pixelType );}

protected:

  PhantomRenderer();
  virtual ~PhantomRenderer();
  
  sov::PhantomInteractor m_Phantom;

  void PrintSelf( std::ostream& , Indent ) const;
  bool m_UpdateRendererOnly;
};

} //end of namespace sov

#endif //__sovPhantomRenderer_h
