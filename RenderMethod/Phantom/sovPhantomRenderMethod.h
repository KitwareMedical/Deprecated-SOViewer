/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovPhantomRenderMethod.h,v $
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
#ifndef __PhantomRenderMethod_h
#define __PhantomRenderMethod_h

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include "sovRenderMethod.h"

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

namespace sov
{

class PhantomRenderMethod
: public RenderMethod<3>
{

public:

  typedef PhantomRenderMethod           Self;
  typedef RenderMethod<3>               Superclass;
  typedef SmartPointer< Self >          Pointer;
  typedef SmartPointer< const Self >    ConstPointer;

  typedef Superclass::ObjectType        ObjectType;
  typedef Superclass::ObjectListType    ObjectListType;
  
  itkNewMacro( PhantomRenderMethod );
  
  virtual void draw(){};

  itkTypeMacro( PhantomRenderMethod, RenderMethod3D );
  
  bool IsSupported( const char* object ) const;

  void RemoveVisualObject( VisualObjectType * object );

  void ApplyTransform(VisualObjectType * object);
  void ChangeProperty(VisualObjectType* object);
  void ChangeRepresentationType(VisualObjectType* object);

  virtual void Update();
    
  std::list<gwpTransform*> & GetActorList(void) {return m_ActorList;}
  std::list<gwpTransform*> & GetActorToBeRemovedList(void) {return m_ActorToBeRemovedList;}
  std::list<gwpTransform*> & GetActorToBeAddedList(void) {return m_ActorToBeAddedList;}

  void RemoveActor(VisualObjectType* actor);
  void ClearActorToBeRemovedList(void);
  void ClearActorToBeAddedList(void){m_ActorToBeAddedList.clear();}

protected:

  PhantomRenderMethod();
  ~PhantomRenderMethod();
 
  void PrintSelf (std::ostream &os, Indent indent) const;
    
  std::list<gwpTransform*> m_ActorList;
  std::list<gwpTransform*> m_ActorToBeAddedList;
  std::list<gwpTransform*> m_ActorToBeRemovedList;
};

}
#endif //__PhantomRenderMethod_h
