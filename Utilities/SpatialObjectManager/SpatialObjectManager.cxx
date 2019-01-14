/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: SpatialObjectManager.cxx,v $
  Language:  C++
  Date:      $Date: 2005-01-27 23:31:57 $
  Version:   $Revision: 1.1.1.1 $

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkSpatialObjectManager_txx
#define _itkSpatialObjectManager_txx

#include "SpatialObjectManager.h"
#include "itkAffineTransform.h"
#include <stdio.h>

/** Default constructor */
SpatialObjectManager
::SpatialObjectManager()
{
  pellipse = new Fl_Pixmap(ellipse);
  ptube = new Fl_Pixmap(tube);
  ptubenet = new Fl_Pixmap(tube_net);
  tree->draw_lines(1);
  m_CurrentLevel = 0;
  m_Interactor = NULL;
}


/** Destructor */
SpatialObjectManager
::~SpatialObjectManager()
{

}

/** Select Object */
void SpatialObjectManager
::SelectObject(void)
{
  int cy;
  int depth;
  Fl_ToggleNode* tnode = (Fl_ToggleNode*) static_cast<Fl_Tree*>(tree)->find(Fl::event_y(),cy,depth);
  if(tnode)
    {
    ShowProperties(static_cast<itk::SpatialObject<>*>(tnode->data()));
    if(m_Interactor)
      {
      m_Interactor->Select(static_cast<itk::SpatialObject<>*>(tnode->data()));
      }
    }
}


/** Show Properties */
void SpatialObjectManager
::ShowProperties(itk::SpatialObject<>* object)
{
  PropertyBrowser->clear();
  char* text = new char[255];
  sprintf(text,"Object name: %s",(char*)object->GetProperty()->GetName().c_str());
  PropertyBrowser->add(text);
  sprintf(text,"Object type: %s",(char*)object->GetTypeName());
  PropertyBrowser->add(text);
  sprintf(text,"Object Id: %d",object->GetId());
  PropertyBrowser->add(text);
  sprintf(text,"Object Parent Id: %d",object->GetParentId());
  PropertyBrowser->add(text);
  //sprintf(text,"Object Pointer: %s",object);
  //PropertyBrowser->add(text);
}




/** Create the tree*/
void SpatialObjectManager
::CreateTree(itk::SpatialObject<3>* object,int level)
{
  Fl_ToggleNode* tr = NULL;

  while((level < m_CurrentLevel) && (m_CurrentLevel>=0))
  {
    tree->traverse_up();
    m_CurrentLevel--;
  }

  std::string name = object->GetProperty()->GetName();
  if(name.length()<3)
    {
    name = object->GetTypeName();
    }


  if(!strcmp(object->GetTypeName(),"GroupSpatialObject"))
  {
    if(level == m_CurrentLevel)
    {
      tr =  tree->add_next((char*)name.c_str(), 1, pellipse);
    }
    else
    {
      tr =  tree->add_sub((char*)name.c_str(), 1, pellipse);
    }
  }
  else if( (!strcmp(object->GetTypeName(),"ImageSpatialObject"))
         )
  {
    if(level == m_CurrentLevel)
    {
      tr = tree->add_next((char*)name.c_str(), 0, pellipse);
    }
    else
    {
      tr = tree->add_sub((char*)name.c_str(), 0, pellipse);
    }
  }
  else if(!strcmp(object->GetTypeName(),"TubeSpatialObject"))
  {
    if(level == m_CurrentLevel)
    {
      tr = tree->add_next((char*)name.c_str(), 0, ptube);
    }
    else
    {
      tr = tree->add_sub((char*)name.c_str(), 0, ptube);
    }
  }
  else if(!strcmp(object->GetTypeName(),"EllipseSpatialObject"))
  {
    if(level == m_CurrentLevel)
    {
      tr = tree->add_next((char*)name.c_str(), 0, pellipse);
    }
    else
    {
      tr = tree->add_sub((char*)name.c_str(), 0, pellipse);
    }
  }
  else if(!strcmp(object->GetTypeName(),"BlobSpatialObject"))
  {
    if(level == m_CurrentLevel)
    {
      tr = tree->add_next((char*)name.c_str(), 0, pellipse);
    }
    else
    {
      tr = tree->add_sub((char*)name.c_str(), 0, pellipse);
    }
  }

  if(tr)
    {
    tr->data(object);
    }

  m_CurrentLevel = level;

  itk::SpatialObject<3>::ChildrenListType* m_ChildrenList = object->GetChildren(0);
  itk::SpatialObject<3>::ChildrenListType::const_iterator it = m_ChildrenList->begin(); 
  while(it != m_ChildrenList->end())
  {
    CreateTree(*it,level+1);
    it++;
  }
  delete m_ChildrenList;

}


/** Update */
void SpatialObjectManager
::Update(void)
{
  if(!m_Scene)
  {
    std::cout << "No Scene plugged in !" << std::endl;
    return;
  }
 
  // delete the tree each time to reconstruct (maybe not the easiest way)
  tree->clear();

  itk::SceneSpatialObject<3>::ObjectListType* m_ChildrenList = m_Scene->GetObjects(0);
  itk::SceneSpatialObject<3>::ObjectListType::const_iterator it = m_ChildrenList->begin();

  m_CurrentLevel = 0;
  while(it != m_ChildrenList->end())
  {
    tree->traverse_start();
    CreateTree(*it,0);
    it++;
  }
   
  delete m_ChildrenList;
}


#endif
