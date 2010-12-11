#include <cassert>
#include "ipp4r.h"


// -------------------------------------------------------------------------- //
// Local functions
// -------------------------------------------------------------------------- //
/**
 * The purpose of this function is to check the metatype infrastructure.
 * If it compiles, then everything is OK.
 */
static void metatype_static_check(void) {
  /* check M_INDEX */
#define CHECK_M_INDEX(INDEX, ARG) STATIC_ASSERT(( M_INDEX(ARX_ARRAY_ELEM(INDEX, M_SUPPORTED)) == INDEX ));
  ARX_ARRAY_FOREACH(ARX_INDEX_ARRAY(ARX_ARRAY_SIZE(M_SUPPORTED)), CHECK_M_INDEX, ~)

  /* check D_INDEX */
#define CHECK_D_INDEX(INDEX, ARG) STATIC_ASSERT(( D_INDEX(ARX_ARRAY_ELEM(INDEX, D_SUPPORTED)) == INDEX ));
  ARX_ARRAY_FOREACH(ARX_INDEX_ARRAY(ARX_ARRAY_SIZE(D_SUPPORTED)), CHECK_D_INDEX, ~)

  /* check C_INDEX */
#define CHECK_C_INDEX(INDEX, ARG) STATIC_ASSERT(( C_INDEX(ARX_ARRAY_ELEM(INDEX, C_SUPPORTED)) == INDEX ));
  ARX_ARRAY_FOREACH(ARX_INDEX_ARRAY(ARX_ARRAY_SIZE(C_SUPPORTED)), CHECK_C_INDEX, ~)

  /* check accessors */
#define CHECK_M_ACCESSORS(METATYPE, ARG) STATIC_ASSERT(( M_CENUM(M_CREATE(M_DATATYPE(METATYPE), M_CHANNELS(METATYPE))) == M_CENUM(METATYPE) ));
  ARX_ARRAY_FOREACH(M_SUPPORTED, CHECK_M_ACCESSORS, ~)
}


// -------------------------------------------------------------------------- //
// ippmetatype_channels
// -------------------------------------------------------------------------- //
IppChannels metatype_channels(IppMetaType metaType) {
#define METAFUNC(C, ARGS) C_CENUM(M_CHANNELS(C))
  IPPMETACALL(metaType, return, M_SUPPORTED, METAFUNC, ~, Unreachable(), -1);
#undef METAFUNC
}


// -------------------------------------------------------------------------- //
// metatype_datatype
// -------------------------------------------------------------------------- //
IppDataType metatype_datatype(IppMetaType metaType) {
#define METAFUNC(D, ARGS) D_CENUM(M_DATATYPE(D))
  IPPMETACALL(metaType, return, M_SUPPORTED, METAFUNC, ~, Unreachable(), -1);
#undef METAFUNC
}


// -------------------------------------------------------------------------- //
// metatype_compose 
// -------------------------------------------------------------------------- //
IppMetaType metatype_compose(IppDataType dataType, IppChannels channels) {
#define METAFUNC_C(C, D) M_CENUM(M_CREATE(D, C))
#define METAFUNC_D(D, ARGS) IPPMETACALL(channels, return, C_SUPPORTED, METAFUNC_C, D, Unreachable(), -1)
  IPPMETACALL(dataType, ARX_EMPTY(), D_SUPPORTED, METAFUNC_D, ~, Unreachable(); return -1, ARX_EMPTY());
#undef METAFUNC_D
#undef METAFUNC_C
}


// -------------------------------------------------------------------------- //
// metatype_pixel_size
// -------------------------------------------------------------------------- //
int metatype_pixel_size(IppMetaType metaType) {
#define METAFUNC(M, ARGS) (sizeof(D_CTYPE(M_DATATYPE(M))) * C_CNUMB(M_CHANNELS(M)))
  IPPMETACALL(metaType, return, M_SUPPORTED, METAFUNC, ~, Unreachable(), -1);
#undef METAFUNC
}


// -------------------------------------------------------------------------- //
// is_channels_supported
// -------------------------------------------------------------------------- //
int is_channels_supported(IppChannels channels) {
  IPPMETACALL(channels, return, C_SUPPORTED, IPPMETAFUNC, (0?ipp, :TRUE, ARX_EMPTY()), ARX_EMPTY(), FALSE);
}


// -------------------------------------------------------------------------- //
// is_datatype_supported
// -------------------------------------------------------------------------- //
int is_datatype_supported(IppDataType dataType) {
  IPPMETACALL(dataType, return, D_SUPPORTED, IPPMETAFUNC, (0?ipp, :TRUE, ARX_EMPTY()), ARX_EMPTY(), FALSE);
}


// -------------------------------------------------------------------------- //
// is_metatype_supported
// -------------------------------------------------------------------------- //
int is_metatype_supported(IppMetaType metaType) {
  IPPMETACALL(metaType, return, M_SUPPORTED, IPPMETAFUNC, (0?ipp, :TRUE, ARX_EMPTY()), ARX_EMPTY(), FALSE);
}