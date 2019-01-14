#if !defined __ColorSchemeXML_h
#define __ColorSchemeXML_h

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <sovObjectViewerWin32Header.h>
#include <itkXMLFile.h>
#include <itksys/SystemTools.hxx>
#include "itkMetaDataObject.h"
#include "itkIOCommon.h"
#include <sovColorScheme.h>
#include <assert.h>


namespace sov
{
  class ColorScheme ;

  class SOViewer_EXPORT ColorSchemeXMLWriter :  public itk::XMLWriterBase<ColorScheme>
  {
public:
  /** standard typedefs */
  typedef itk::XMLWriterBase<ColorScheme> Superclass;
  typedef ColorSchemeXMLWriter Self;
  typedef itk::SmartPointer<Self> Pointer;
  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  /** Run-time type information (and related methods). */
  itkTypeMacro(ColorSchemeXMLWriter ,    itk::XMLWriterBase<ColorScheme>);

  /** Test whether a file is writable. */
  virtual int CanWriteFile(const char* ) { return true; };
  /** Actually write out the file in question */
  virtual int WriteFile() ;

  };

  class SOViewer_EXPORT ColorSchemeXMLReader : public itk::XMLReader<ColorScheme>
  {
  public:
    /** Standard typedefs */ 
    typedef ColorSchemeXMLReader Self;
    typedef itk::XMLReader<ColorScheme> Superclass;
    typedef itk::SmartPointer<Self> Pointer;
    /** Run-time type information (and related methods). */
    itkTypeMacro(ColorSchemeXMLReader, XMLReader);
    /** Method for creation through the object factory. */
    itkNewMacro(Self);
  
    /** Determine if a file can be read */
    virtual int CanReadFile(const char* name);

    void SetObject (ColorScheme *pColorScheme) ;
  protected:
    virtual void StartElement(const char * name,const char **atts);
    virtual void EndElement(const char *name);
    virtual void CharacterDataHandler(const char *inData, int inLength);
  private:
    std::string m_CurCharacterData;
    ColorScheme *m_ColorScheme ;
  };

}

#endif // !defined __ColorSchemeXML_h

