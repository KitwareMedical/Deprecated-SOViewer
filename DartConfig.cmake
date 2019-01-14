#
# Dart server to submit results (used by client)
#
SET (NIGHTLY_START_TIME "21:00:00 EDT")

SET (DROP_SITE "www.cdash.org")
#SET (DROP_LOCATION "/cgi-bin/HTTPUploadDartFile.cgi")
SET (DROP_LOCATION "/CDashPublic/submit.php?project=SOViewer")
SET (DROP_METHOD "http")
SET (TRIGGER_SITE "http://${DROP_SITE}/cgi-bin/SubmitSOViewer.cgi")


# Project Home Page
SET (PROJECT_URL "http://public.kitware.com/SOViewer")

#
# Dart server configuration 
#
SET (CVS_WEB_URL "http://public.kitware.com/cgi-bin/viewcvs.cgi/")
SET (CVS_WEB_CVSROOT "SOViewer")

SET (USE_GNATS "On")
SET (GNATS_WEB_URL "http://www.itk.org/Bug/query.php?projects=17")

OPTION(BUILD_DOXYGEN "Build source documentation using doxygen" "On")
SET (DOXYGEN_CONFIG "${PROJECT_BINARY_DIR}/doxygen.config" )
SET (USE_DOXYGEN "On")
SET (DOXYGEN_URL "http://caddlab.rad.unc.edu/Public/Dashboard/SOViewer-Dashboard/Documentation/Doxygen/html/" )


#
# Copy over the testing logo
#
CONFIGURE_FILE(${SOV_SOURCE_DIR}/TestingLogo.gif ${SOV_BINARY_DIR}/Testing/HTML/TestingResults/Icons/Logo.gif COPYONLY)

# Email delivery on broken builds
# DeliverBrokenBuildEmail should be a list of models (e.g. Nightly)
#   for which email should be sent. If this variable is set, _all_
#   the other options must be set.
# SMTPMailhost: the outgoing mail server
#   when necessary
# EmailFrom: the from address of the email
# DartboardBaseURL: the "Testing" directory of the dashboard URL
# EmailProjectName: the name of the project
# BuildMonitors: a list of people to whom the email if cc'ed. Each element
#   of the list has three parts: site regexp, buildname regexp, and set of
#   email addresses. For example,
#     BuildMonitors: {.* .* A} {foo\.bar .* B} {.* WinNT C}
#   would cause A to be notified of all build errors, B of all errors with
#   builds from *.foo.bar, and C of all errors on WinNT.
# CVSIdentToEmail: a list of rules for converting CVS identities into
#   email addresses. A list of pairs of {regexp substitution} are
#   used. The first regexp to match specifies which subsitution to use.
#   For example
#     CVSIndentToEmail: {robotuser DO_NOT_EMAIL} {.* {\0@somewhere.net} }
#   would mean that robotuser is not mailed, and every other user is
#   at somehwere.net
# DeliverBrokenBuildEmailWithConfigureFailures: Set to 1 if
#   you want email delivered for Build Warnings (Normally 1)
# DeliverBrokenBuildEmailWithBuildErrors: Set to 1 if you want
#   email delivered for Build Errors (Normally 1)
# DeliverBrokenBuildEmailWithBuildWarnings: Set to 1 if you
#   want email delivered for Build Warnings (Normally 0)
# DeliverBrokenBuildEmailWithTestNotRuns: Set to 1 if you
#   want email delivered for Build Warnings (Normally 0)
# DeliverBrokenBuildEmailWithTestFailures: Set to 1 if you
#   want email delivered for Build Warnings (Normally 0)
#
# The old "ContinuousEmail" effect can be obtained by setting
#   DeliverBrokenBuildEmail: Continuous
#   EmailFrom: $ContinuousFrom
#   DartboardBaseURL: $ContinuousBaseURL
#   EmailProjectName: $ContinuousProject
#   BuildMonitors: {.* .* "$ContinuousMonitorList"}
#   CVSIdentToEmail: {.* {\0@$DefaultContinuousDomain} }
#   DeliverBrokenBuildEmailWithConfigureFailures: 0
#   DeliverBrokenBuildEmailWithBuildErrors: 1
#   DeliverBrokenBuildEmailWithBuildWarnings: 0
#   DeliverBrokenBuildEmailWithTestNotRuns: 0
#   DeliverBrokenBuildEmailWithTestFailures: 0

# DeliverBrokenBuildEmail: Nightly
# EmailFrom: dashboard@caddlab.rad.unc.edu
# DartboardBaseURL: http://caddlab.rad.unc.edu/Public/Dashboard/SOViewer-Dashboard
# EmailProjectName: SOViewer
# BuildMonitors: jomier@unc.edu
# CVSIdentToEmail: 
# DeliverBrokenBuildEmailWithConfigureFailures: 1
# DeliverBrokenBuildEmailWithBuildErrors: 1
# DeliverBrokenBuildEmailWithBuildWarnings: 1
# DeliverBrokenBuildEmailWithTestNotRuns: 1
# DeliverBrokenBuildEmailWithTestFailures: 1

# Continuous email delivery variables
# DeliverContinuousEmail should be On/Off
# If DeliverContinuousEmail is On, _all_ of the other options must be set!
# These variables are from the server's perspective, not the client.
# DeliverContinuousEmail: OFF
# ContinuousFrom: jomier@unc.edu
# DefaultContinuousDomain: 
# SMTPMailhost: smtp.med.unc.edu
# ContinuousMonitorList: jomier@unc.edu
# ContinuousBaseURL: http://caddlab.rad.unc.edu/Public/Dashboard/SOViewer-Dashboard
# ContinuousProject: SOViewer
