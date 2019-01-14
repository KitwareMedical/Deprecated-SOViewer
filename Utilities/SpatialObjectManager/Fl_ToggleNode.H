/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: Fl_ToggleNode.H,v $
  Language:  C++
  Date:      $Date: 2006-02-28 17:42:39 $
  Version:   $Revision: 1.3 $

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef Fl_ToggleNode_H
#define Fl_ToggleNode_H

#include <Fl_Node.H>
#include <string.h>

class Fl_Pixmap;

class Fl_ToggleNode : public Fl_Node {

  friend class Fl_ToggleTree;

public:

  Fl_ToggleNode(char* label = 0, int can_open = 1, Fl_Pixmap* pixmap = 0,
                void * d = 0) : Fl_Node() {
    vsub_ = 0;
    selected_ = 0;
    changed_ = 0;
    opened_ = 1;

    label_ = strdup(label);
    pixmap_ = pixmap;
    can_open_ = can_open;
    data_ = d;
  }

  char* label(void) {
    return label_;
  }

  void label(char* ptr) {
    if (label_)
      delete label_;
    label_ = strdup(ptr);
  }

  Fl_Pixmap* pixmap(void) {
    return pixmap_;
  }

  void pixmap(Fl_Pixmap* ptr) {
    pixmap_ = ptr;
  }

  void* data() const {
    return data_;
  }

  void data(void* v) {
    data_ = v;
  }

  int can_open() {
    return can_open_;
  }

  void can_open (int b) {
    can_open_ = b;
  }

  int is_open() {
    return opened_;
  }

protected:

  int selected_;
  int changed_;
  int can_open_;

  char* label_;
  Fl_Pixmap* pixmap_;
  void* data_;

};

#endif
