#include <math.h>
#include <sovColorSchemeXML.h>

namespace sov
{
  int ColorSchemeXMLWriter::WriteFile() 
  {
    // sanity checks
    if ((m_InputObject == 0)||(m_Filename.length() == 0))
    {
      return 0 ;
    }
    std::ofstream output(m_Filename.c_str());
    if(output.fail())
    {
      return 0 ;
    }

    // get the color mode before starting to mess around with it
    enum ColorScheme::colorMode mode = m_InputObject->GetMode () ;

    // write out the header
    WriteStartElement("?xml version=\"1.0\"?",output);
    output << std::endl;
    WriteStartElement("!DOCTYPE COLORSCHEME",output);
    output << std::endl;


    WriteStartElement("COLORSCHEME",output);

    // solid settings
    WriteStartElement("SolidSettings",output);
        float h, s, v ;
        m_InputObject->GetColor (3, 0, h, s, v) ;
        WriteStartElement ("Color", output) ;
        output << "0 " << h << " " << s << " " << v ;
        WriteEndElement ("Color", output ) ;
        output << std::endl;


    WriteEndElement("SolidSettings",output);
    output << std::endl;

    // significance map settings
    WriteStartElement("SigMapSettings",output);
    // temporarily change active mode
    m_InputObject->SetMode (ColorScheme::SIG_MAP) ;

    WriteStartElement ("Threshold", output) ;
          float f = m_InputObject->GetThreshold() ;
          output << "0 " << f ;
        WriteEndElement ("Threshold", output) ;
        output << std::endl;

        m_InputObject->GetColor (1, 0, h, s, v) ;
        WriteStartElement ("Color", output) ;
          output << "0 " << h << " " << s << " " << v ;
        WriteEndElement ("Color", output) ;
        output << std::endl;
        m_InputObject->GetColor (1, 1, h, s, v) ;
        WriteStartElement ("Color", output) ;
          output << "1 " << h << " " << s << " " << v ;
        WriteEndElement ("Color", output) ;
        output << std::endl;
        m_InputObject->GetColor (1, 2, h, s, v) ;
        WriteStartElement ("Color", output) ;
          output << "2 " << h << " " << s << " " << v ;
        WriteEndElement ("Color", output) ;
        output << std::endl;

    WriteEndElement("SigMapSettings",output);
    output << std::endl;

    // distance map settings
    WriteStartElement("DistMapSettings",output);
    // temporarily change active mode
    m_InputObject->SetMode (ColorScheme::DIST_MAP) ;

        WriteStartElement ("Threshold", output) ;
          f = m_InputObject->GetThreshold(0) ;
          output << "0 " << f ;
        WriteEndElement ("Threshold", output) ;
        output << std::endl;
        WriteStartElement ("Threshold", output) ;
          f = m_InputObject->GetThreshold(1) ;
          output << "1 " << f ;
        WriteEndElement ("Threshold", output) ;
        output << std::endl;

        m_InputObject->GetColor (2, 0, h, s, v) ;
        WriteStartElement ("Color", output) ;
          output << "0 " << h << " " << s << " " << v ;
        WriteEndElement ("Color", output) ;
        output << std::endl;
        m_InputObject->GetColor (2, 1, h, s, v) ;
        WriteStartElement ("Color", output) ;
          output << "1 " << h << " " << s << " " << v ;
        WriteEndElement ("Color", output) ;
        output << std::endl;

    WriteEndElement("DistMapSettings",output);
    output << std::endl;

    // write out the originally active mode
    WriteStartElement("ActiveMode",output);
    if ( mode == ColorScheme::SOLID )
      output << "Solid" ;
    else if ( mode == ColorScheme::SIG_MAP )
      output << "SignificanceMap" ;
    else
      output << "DistanceMap" ;

    WriteEndElement("ActiveMode",output);
    output << std::endl;

    // restore the originally active mode
    m_InputObject->SetMode ( mode ) ;


    // done - cleanup
    WriteEndElement("COLORSCHEME",output);
    output << std::endl;
    output.close();

    return 0;
  };


  int ColorSchemeXMLReader::CanReadFile(const char *name)
  {
    if(!itksys::SystemTools::FileExists(name) ||
      itksys::SystemTools::FileIsDirectory(name) ||
      itksys::SystemTools::FileLength(name) == 0)
      return 0;
    return 1;
  }

  void ColorSchemeXMLReader::StartElement(const char * name,const char ** itkNotUsed(atts))
  {
    //cout << "Start " << name << endl ;
    if(strcmp (name, "SolidSettings" )== 0)
    {
      m_ColorScheme->SetMode ( ColorScheme::SOLID ) ;
    }
    else if(strcmp (name, "SigMapSettings" )== 0) 
    {
      m_ColorScheme->SetMode ( ColorScheme::SIG_MAP ) ;
    }
    else if(strcmp (name, "DistMapSettings" )== 0) 
    {
      m_ColorScheme->SetMode ( ColorScheme::DIST_MAP ) ;
    }
  }
  
  void ColorSchemeXMLReader::EndElement(const char *name)
  {
    //cout << "End " << name << endl ;
    //itk::MetaDataDictionary &thisDic=m_OutputObject->GetMetaDataDictionary();
    if(strcmp(name,"Color") == 0) 
    {
      int index ;
      float h, s, v ;
      const char *current ;
      index = atoi (m_CurCharacterData.c_str()) ;
      
      current = strchr (m_CurCharacterData.c_str(), ' ') + 1 ;
      h = atof ( current ) ;

      current = strchr (current, ' ') + 1 ;
      s = atof ( current ) ;

      current = strchr (current, ' ') + 1 ;
      v = atof ( current ) ;

      m_ColorScheme->SetColorHSV (index, h, s, v) ;
    }
    else if ( strcmp (name, "Threshold" ) == 0 )
    {
      int index ;
      float t ;
      const char * current ;
      index = atoi ( m_CurCharacterData.c_str()) ;

      current = strchr (m_CurCharacterData.c_str(), ' ') + 1 ;
      t = atof ( current ) ;
      m_ColorScheme->SetThreshold (index, t) ;
    }
    else if ( strcmp (name, "ActiveMode") == 0 )
    {
      if ( strcmp (m_CurCharacterData.c_str (), "Solid") == 0 )
      {
        m_ColorScheme->SetMode ( m_ColorScheme->SOLID ) ;
      }
      else if ( strcmp (m_CurCharacterData.c_str(), "SignificanceMap") == 0 )
      {
        m_ColorScheme->SetMode ( m_ColorScheme->SIG_MAP ) ;
      }
      else if ( strcmp (m_CurCharacterData.c_str(), "DistanceMap") == 0 )
      {
        m_ColorScheme->SetMode ( m_ColorScheme->DIST_MAP ) ;
      }
    }
  }
  
  void ColorSchemeXMLReader::CharacterDataHandler(const char *inData, int inLength)
  {
    m_CurCharacterData = "";
    for(int i = 0; i < inLength; i++)
      m_CurCharacterData = m_CurCharacterData + inData[i];
    //cout << m_CurCharacterData << endl ;
  }

  void ColorSchemeXMLReader::SetObject (ColorScheme *pColorScheme)
  {
    m_ColorScheme = pColorScheme ;
  }
};

