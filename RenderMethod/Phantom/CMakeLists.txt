IF(USE_PHANTOM)
    ADD_DEFINITIONS(-DUSE_PHANTOM)
    AUX_SOURCE_DIRECTORY(
      .
      sovPhantomRenderMethod_SRCS
    )

ADD_LIBRARY(
  SOVPhantomRenderMethod ${sovPhantomRenderMethod_SRCS}
)

ENDIF(USE_PHANTOM)