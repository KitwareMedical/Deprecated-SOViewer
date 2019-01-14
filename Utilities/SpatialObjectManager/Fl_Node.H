/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: Fl_Node.H,v $
  Language:  C++
  Date:      $Date: 2005-01-27 23:31:57 $
  Version:   $Revision: 1.1.1.1 $

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef Fl_Node_H
#define Fl_Node_H

class Fl_Node {

  friend class Fl_Tree;

public:

  Fl_Node() {
    prev_ = 0;
    next_ = 0;
    sub_ = 0;
    vsub_ = 0;
    up_ = 0;
    opened_ = 0;
  }

protected:

  Fl_Node* prev_;
  Fl_Node* next_;
  Fl_Node* sub_;
  Fl_Node* vsub_;
  Fl_Node* up_;

  int opened_;

};

#endif
