#!/usr/bin/env perl

#
#
# This Script is used to infer figure dependencies from the .cxx/.txx source
# files in the Insight/Examples directory.
# 
# This automatic generation mechanism guaranties that the figures 
# presented on the SoftwareGuide book match exactly the code
# the is compiled and updated in the source repository.
#
# To automate generation, the example source files must contain tags such as
#   .
#   .
# //  Software Guide : BeginCommandLineArgs
# //    INPUTS: {BrainProtonDensitySlice.png}, {BrainProtonDensitySliceR13X13Y17.png}
# //    INPUTS: {SomeOtherInputfile.png}, {MoreInputfiles.png}
# //    OUTPUTS: {GradientDiffusedOutput.png}, {Output2.txt}
# //    5 0.25 3
# //  Software Guide : EndCommandLineArgs
#  .
#  .
#
# Multiple tags may be specified.
# 
# Please do not specify paths along with the file names. A list of search paths 
# where input data files may be found is specified through CMAKE. Paths are 
# specified in a colon seperated list such as
#     /Insight/Examples/Data:/VTK/VTKData
# Specifying the root path will suffice. A recursive search for input data 
# is done.
#
# The output of this script is a set of "ExampleSourceFileName.cmake" file. THe
# file contains a set of macros used by CMake to infer figure dependencies, 
# possibly across source files. ie. One example may be used to generate a 
# figure used as input to another example.
# 
# Dependencies for a figure are only inferred if the figure is included 
# in the software guide with the \includegraphics tag.
# 
# Generated figures are stored in Art/Generated.
# 
# The script is invoked from CMAKE if BUILD FIGURES is ON
#
# If BUILD FIGURES is OFF, it is expected that the images be present in either
# (.png, .fig, .jpg, .eps) format in the Art/ folder
#
# Resolving the flipping images issue:
# CMakeLists contains a list of the input files whose eps files need to be flipped
# when included in the SW guide. Their corresponding output eps files will be flipped as
# well during inclusion in the SW guide
# Note that the eps files are flipped, not the inputs or the outputs themselves. The
# files that are used in the command line arguments etc are the original ones. In other 
# words every image that you see in the SW guide that is the same as or is generated
# from the list of images in teh CMakeLists file is a flipped version!
# 
use File::Spec; #for platform independent file paths
use File::Find; #for platform independent recursive search of input images in 
                #specified directories
use File::Copy;
use IO::File;


#
# Tag defs
# 
my $beginCmdLineArgstag = "BeginCommandLineArgs";
my $endCmdLineArgstag   = "EndCommandLineArgs";
my $fileinputstag = "INPUTS:";
my $fileoutputstag = "OUTPUTS:";
my $includegraphicstag = 'includegraphics';


$numArgs = $#ARGV + 1;
if( $numArgs < 5 )
  {
  print "Usage arguments: \n".
  "  Name of the .cxx/.txx file (with extenstion).\n".
  "  ITKExecsDirectoryPath \n".
  "  Cmake file to be generated\n".
  "  Name of the TEX file generated, so dependencies can be specified\n".
  "  Ouput folder to store generated images\n".
  "  Colon separated list of possible include directories for input images\n".
  die;
  }

@searchdir = split(/:/, $ARGV[2]);
foreach $eachpath (@searchdir)
  {
    if (-d File::Spec->canonpath($eachpath))   # if the directory is valid
    { push (@searchdirs, File::Spec->canonpath($eachpath)); }
  }
GetArgsAndFilenames( $ARGV[1], $ARGV[0], \@searchdirs, $ARGV[3], $ARGV[4], $ARGV[5]);


#
#  Subroutine for parsing each one of the .cxx files.
#  The command line args and Figure file names are returned
#
sub GetArgsAndFilenames {

  my $execpath = shift;
  my $inputfilename  = shift;
  my $searchdirs = shift;
  my $cmakefile = shift;
  my $texfile = shift;
  my $generatedPath = shift;
  my $source_file =  $inputfilename;
  
  my $examplefilename = File::Spec->canonpath($inputfilename);
  if (!(-e $examplefilename)) 
    {
    die "File $examplefilename does not exist.\n"; 
    }


  #Strip the path and file extension to get the exec file name.
  #Exec file anme is assumed to be the same as the source filename
  my $volume; my $directories;
  ($volume,$directories,$inputsourcefile) = File::Spec->splitpath( $inputfilename );
  $inputsourcefile =~ m/(.*)(\.cxx|\.CXX|\.txx|\.TXX)/; 
  my $execfilenamebase = $1;

  #If the executable path has a leading /, remove it.
  if ($execpath =~ /\/$/)
    {
    $execpath =~ m/(.*)\//; 
    $execpath = $1;
    }
  if ($generatedPath =~ /\/$/)
    {
    $generatedPath =~ m/(.*)\//; 
    $generatedPath = $1;
    }

  # Make the path platform independent
  my @execdirs = File::Spec->splitdir($execpath);
  $execfilename = File::Spec->catfile(@execdirs, $execfilenamebase);

  #Get the command line args from the source file
  open(INFILE, "< $examplefilename"  )  or die "Can't open $examplefilename $!";

  my $tagfound     = 0;
  my $filefound = 0;
  my $cmdLineArgs;
  my $thisline='';
  my $counter=0; 
  my $inputfileInThisLine;
  my $outputfileInThisLine;
  my @outputfilesInThisLine;
  my $artdir;
  my @outputs;
  
  #Create a .cmake file
  $cmakeFile =  File::Spec->canonpath($cmakefile);

  #Check if file exists
  if (-e $cmakeFile) 
    {
    open CMAKEFILE, "<  $cmakeFile" or die "Couldn't open $cmakeFile";
    @cmakelinesold = <CMAKEFILE>;   
    }
    
  
  #Check if there are any input files that need to be flipped.
  $generatedPath =  File::Spec->canonpath($generatedPath);
  $flipfilename = File::Spec->catfile($generatedPath,"Flipped_files.txt");
  @flippedfiles = ();
  if (-e $flipfilename)
    {
    open FLIPFILE, "< $flipfilename" or die "Could not open $flipfilename";
    @flippedfiles = <FLIPFILE>;
    close FLIPFILE;
    }

    
  #
  #Read each line and Parse the input file 
  #
  while(<INFILE>) 
    {
    $thisline=$_; 
    if ($thisline)
      {
        
      if ($thisline =~ /$beginCmdLineArgstag/)
        { 
        $tagfound = 1;
        $cmdLineArgs = '';
        @outputs = (); @inputs = (); @byproducts = ();
        @generatedinputfile = ();
        }
        
      elsif ($thisline =~ /$endCmdLineArgstag/)
        {
        # Add these commands to the .cmake file
        $tagfound=0;
        # Execute with command line args now.. 
        $toexecute = "$execfilename"." "."$cmdLineArgs";
        foreach $output (@outputs)
          { 
          foreach $generatedinput (@generatedinputfile)
            {
            $myline = sprintf("#This figure was generated by another example.. so add a dependency\n");
            push(@cmakelines, $myline);
            $myline = sprintf("ADD_GENERATED_FIG_DEPS( \"%s\" \"%s\" )\n",$output,$generatedinput);
            push(@cmakelines, $myline);
            }
          $myline = sprintf("# Cmake macro to invoke: %s\n",$toexecute);
          push(@cmakelines, $myline);
          $myline = sprintf("RUN_EXAMPLE( \"%s\" \"%s\" \"%s\" %s )\n",$execfilenamebase,$output, $source_file, $cmdLineArgs);
          push(@cmakelines, $myline);
          }
        if (-e $flipfilename)
          {
          CheckForFlippedImages(\@inputs, \@outputs, $flipfilename);
          }
        }
      
      #        
      #Read and parse each line of the command line args
      #
      if ($tagfound)
        {
          
        if (!($thisline =~ /$beginCmdLineArgstag/))
          {
          $_ =~ s/\/\///; 
          chomp;
          $line = $_;

          #
          #Line contains file inputs
          #
          if ($thisline =~ /$fileinputstag/)
            {
            @inputfilesInThisLine = GetInputFilesInThisLine($line);

            # Search the path tree to get the full paths of the input files.
            foreach $inputfileInThisLine (@inputfilesInThisLine)
              {
              $inputfileInThisLine =~ m/{(.*)}/;
              $inputfileInThisLine = $1;
              push(@inputfiles, $inputfileInThisLine);
              if (($inputfileInThisLine =~ /{/) || ($inputfileInThisLine =~ /}/))
                { die "\nPlease check syntax. Input/Output files must be included ".
                  "in a comma separated list and enclosed in {} as in ".
                  "INPUTS: {file1}, {file2}, .... \n";  
                }       
              $filefound=0;

              foreach $searchelement (@$searchdirs)
                {
                File::Find::find (
                sub 
                  { 
                  if ($File::Find::name =~ /$inputfileInThisLine/) 
                    { 
                    # We found the file in the directory.
                    # Check to see if it is a plain file - not a directory
                    $foundfilename = $File::Find::name; 
                    $filefound = 1;
                    }
                  }, $searchelement);
                  if ($filefound)   { push(@inputs,$inputfileInThisLine); last;  }
                }

              if (!($filefound)) 
                {
                #die " File $inputfileInThisLine could not be found in the search paths supplied.";
                #File must be generated by another source....So must be found in the generatedPath
                $foundfilename = $generatedPath.'/'.$inputfileInThisLine;
                push(@generatedinputfile, $inputfileInThisLine);
                }
              
              #Add the file to the list of command line arguments in the same order
              $cmdLineArgs = $cmdLineArgs . ' ' . File::Spec->canonpath($foundfilename);
              }
            }
            
          #Line contains file outputs
          #
          elsif ($thisline =~ /$fileoutputstag/)
            {
            @outputfilesInThisLine = GetOutputFilesInThisLine($line);
            
            foreach $outputfileInThisLine (@outputfilesInThisLine)
              {
              if ($outputfileInThisLine =~ m/{(.*)}/)
                {
                $outputfileInThisLine = $1;
                push(@outputfiles, $1);
                
                if (($outputfileInThisLine =~ /{/) || ($outputfileInThisLine =~ /}/))
                  { die "\nPlease check syntax. Input/Output files must be included ".
                    "in a comma separated list and enclosed in {} or [] as in ".
                    "OUTPUTS: {file1}, {file2}, [file3] ..... \n";  
                  }       
                
                $tmp =   $generatedPath.'/'.$outputfileInThisLine;
                $outputfiletoadd = File::Spec->canonpath($tmp);  
                  
                #Add the file to the list of command line arguments in the same order
                $cmdLineArgs = $cmdLineArgs . ' ' . $outputfiletoadd;
                push(@outputs, $outputfileInThisLine);  
                }
              elsif ($outputfileInThisLine =~ m/\Q[\E(.*)\Q]\E/)
                { 
                $outputfileInThisLine = $1;
                push(@outputfiles, $1);
                
                if (($outputfileInThisLine =~ /{/) || ($outputfileInThisLine =~ /}/))
                  { die "\nPlease check syntax. Input/Output files must be included ".
                    "in a comma separated list and enclosed in {} or [] as in ".
                    "OUTPUTS: {file1}, {file2}, [file3] ..... \n";  
                  }       
                
                $tmp =   $generatedPath.'/'.$outputfileInThisLine;
                push(@byproducts, $outputfileInThisLine);  
                push(@outputs,$outputfileInThisLine);
                }
              }
            }

          else  #Not a file input, just a command line arg
            {
            $thisLineContains = join(' ', split); 
            $cmdLineArgs = $cmdLineArgs . ' ' . $thisLineContains;
            }    
          }
        }

        
      #
      #Parse file to see the list of eps files generated (through the includegraphics statement)
      #
      if ($thisline =~ /$includegraphicstag/)
        {
        GetLatexFiles($thisline,\@lateximgfile,\@lateximgfilebase);
        }
      }
    }

  
  #
  # Generate CMAKE macros to convert using ImageMagick's convert exec
  #
  ($v,$examplesdir,$f) = File::Spec->splitpath($cmakefile);  my $ctr=0;
  $examplesdir = File::Spec->canonpath($examplesdir);
  push(@iofiles,@outputfiles);   push(@iofiles,@inputfiles);
  foreach $lateximgFile (@lateximgfile)
    {
    $lateximgFilebase = $lateximgfilebase[$ctr++].'.';  
    foreach $cmdlineoutfile (@iofiles)
      {
      if (!(grep(/^$cmdlineoutfile$/, @byproducts)))
        { $filepath = $generatedPath;  }
      else {  $filepath = $examplesdir; }
      print @byrpoducts;
      if ($cmdlineoutfile =~ /$lateximgFilebase/)
        {
        if (ShouldBeFlipped($cmdlineoutfile, $flipfilename))
          {
          if (IsAnInputFile($cmdlineoutfile,$searchdirs,\$path))
            { $myline = sprintf("CONVERT_AND_FLIP_INPUT_IMG( \"%s\" \"%s\" \"%s\" )\n",$cmdlineoutfile,$lateximgFile,$path); }
          else
            { $myline = sprintf("CONVERT_AND_FLIP_IMG( \"%s\" \"%s\" \"%s\" )\n",$cmdlineoutfile,$lateximgFile,$filepath); }
          }
        else
          {
          if (IsAnInputFile($cmdlineoutfile,$searchdirs,\$path))
            { $myline = sprintf("CONVERT_INPUT_IMG( \"%s\" \"%s\" \"%s\" )\n",$cmdlineoutfile,$lateximgFile,$path); }
          else
            { $myline = sprintf("CONVERT_IMG( \"%s\" \"%s\" \"%s\" )\n",$cmdlineoutfile,$lateximgFile,$filepath); }
          }
        push(@cmakelines, $myline);
        $myline = sprintf("ADD_DEP_TEX_ON_EPS_FIGS( \"%s\" \"%s\" )\n",$texfile,$lateximgFile);                 
        push(@cmakelines, $myline);
        }
      }
    }

  if ((!($cmakelinesold)) | (@cmakelines != $cmakelinesold)) {
    $cmakefilehandle = new IO::File $cmakeFile, "w";
    if (!(defined $cmakefilehandle)) { die "Could not open file $cmakeFile\n"; }
    foreach $cmakeline (@cmakelines) {
      $cmakefilehandle->printf("%s",$cmakeline);
      }
    }

  UpdateTableFile($generatedPath, \@iofiles);
  }



#
# Check if the input image in the command line arg is one of the
# flipped ones... if so set the output image as one to be flipped
# ... legacy SoftwareGuide problem
# 
sub CheckForFlippedImages{
  my $inps = shift;
  my $outs = shift;
  my $flipfilename = shift;

  if (-e $flipfilename)
    {
    open FLIPFILE, "< $flipfilename" or die "Could not open $flipfilename";
    while (<FLIPFILE>) { push(@flippedfiles,split); }
    close FLIPFILE;
    }
  else { return 0;  }

  my $matchfound=0;
  
  foreach $inpFile (@$inps)
    { 
    if ($inpFile =~  m/(.*)\./) { $inpFile = $1;  }
    for ($i=0;$i<@flippedfiles;$i++)
      { 
      chomp($flippedfiles[$i]);
      $flippedfiles[$i] =~ s/ //;
      $flippedfiles[$i] =~ m/(.*)\./;
      }
    @matches = grep(/^$inpFile$/, @flippedfiles);
    if (@matches) 
      {
      AddFilesToListOfFlippedFiles(\@$outs, $flipfilename);
      $matchfound=1;
      }
    }
  return $matchfound;
}
  
  
#
# Keep a file containing a list of flipped images... 
# legacy SoftwareGuide problem
# 
sub  AddFilesToListOfFlippedFiles{
  my $outs = shift;
  my $flipfilename = shift;

  if (-e $flipfilename)
    {
    open FLIPFILE, "< $flipfilename" or die "Could not open $flipfilename";
    while (<FLIPFILE>) { push(@flippedfiles,split); }
    close FLIPFILE;
    }
  else { return;  }
  
  foreach $outFile (@$outs)
    { 
    for ($i=0;$i<@flippedfiles;$i++)
      { 
      chomp($flippedfiles[$i]);
      $flippedfiles[$i] =~ s/ //;
      $flippedfiles[$i] =~ m/(.*)\./;
      }
    if ($outFile =~  m/(.*)\./) { $outFile = $1;  }
    @matches = grep(/^$outFile$/, @flippedfiles);
    if (!(@matches)) 
      {
      if (-e $flipfilename)
        {
        open FLIPFILE, ">> $flipfilename" or die "Could not open $flipfilename";
        printf FLIPFILE "$outFile ";
        close FLIPFILE;
        }
      }
    }
  }


#
# Check to see if the image is one that has been set as one to be flipped  
# 
sub ShouldBeFlipped{
  my $file = shift;
  my $flipfilename = shift;

  if (-e $flipfilename)
    {
    open FLIPFILE, "< $flipfilename" or die "Could not open $flipfilename";
    while (<FLIPFILE>) { push(@flippedfiles,split); }
    close FLIPFILE;
    }
  else { return 0;  }

  for ($i=0;$i<@flippedfiles;$i++)
    { 
    chomp($flippedfiles[$i]);
    $flippedfiles[$i] =~ s/ //;
    $flippedfiles[$i] =~ m/(.*)\./;
    }

  if ($file =~  m/(.*)\./) { $file = $1;  }
  @matches = grep(/^$file$/, @flippedfiles);
  if (@matches) { return 1; }
  else { return 0;  }
}

    
#
# Parse the line to see if there are any files included in the latex doc
# with the \includegraphics method. If so, add it to the @lateximgfile array.
# 
sub GetLatexFiles{
  my $thisline = shift;
  my $lateximgfile = shift;
  my $lateximgfilebase = shift;
  
  $thisline =~ m/$includegraphicstag(.*)/; $thisline = $1;
  $thisline =~ m/{(.*)}/;    $thisline = $1;
  $thisline =~ s/ //; 
  $lateximgFile = $1;
  push (@$lateximgfile, $lateximgFile);
  $lateximgFile =~ m/(.*)\./;
  $lateximgFilebase = $1;
  push (@$lateximgfilebase, $lateximgFilebase);
}

sub GetOutputFilesInThisLine{
  my $line = shift;

  $line =~ s/$fileoutputstag//; #Strip the tag away
  # squish more than one space into one
  $line =~ tr/ //s; 
  $line =~ s/^ *//; #strip leading and trailing spaces
  $line =~ s/ *$//; 
  @outputfilesInThisLine = split(/,/,$line);
  return @outputfilesInThisLine;
}

sub GetInputFilesInThisLine{
  my $line = shift;
  $line =~ s/$fileinputstag//; #Strip the tag away
  # squish more than one space into one
  $line =~ tr/ //s; 
  $line =~ s/^ *//; #strip leading and trailing spaces
  $line =~ s/ *$//;
  @inputfilesInThisLine = split(/,/,$line);
  return @inputfilesInThisLine;
}


sub  UpdateTableFile{
  my $generatedPath = shift; my $iofiles = shift;
  if (!(@$iofiles)) { return; } # No generated io files to update 
  $GenFilename = File::Spec->catfile($generatedPath,"GeneratedFiles.txt");
  
  # Get current list
  if (-e $GenFilename)
    {
    open GENFILEHANDLE, "< $GenFilename" or die "Could not open $flipfilename";
    while (<GENFILEHANDLE>) { push(@generatedfiles,split /;/); }
    close GENFILEHANDLE;    
    }

  foreach $ioFile (@$iofiles)
    {
    if ($ioFile =~  m/(.*)\./) { $ioFile = $1;  }
    if ($ioFile) {
      $ioFile = $ioFile.'.*';
      @matches = grep(/^$ioFile$/, @generatedfiles);
      if (!(@matches)) 
        {
        open GENFILEHANDLE, ">> $GenFilename" or die "Could not open $flipfilename";
        printf GENFILEHANDLE "$ioFile;";
        close GENFILEHANDLE;
        }
      }
    }
  }
  
  
sub IsAnInputFile{
  my $file = shift; my $datapaths = shift;
  my $path_ptr = shift;
  my $isAnInputFile = 0;

  foreach $datapath (@$datapaths)
    {  
    File::Find::find (
    sub 
      { 
      if ($File::Find::name =~ /$file/) 
        { 
        # We found the file in the directory.
        # Check to see if it is a plain file - not a directory
        $$path_ptr = File::Spec->canonpath($datapath); 
        $isAnInputFile = 1;
        }
      }, $datapath);
      if ($isAnInputFile) { last; }
    }
  return $isAnInputFile;
}
 
  
