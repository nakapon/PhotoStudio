#pragma once

#ifndef PF_SAFE_DELETE
#define PF_SAFE_DELETE(p)       { if((p) != nullptr){ delete (p); (p) = nullptr; } }
#endif

#ifndef PF_SAFE_DELETE_ARRAY
#define PF_SAFE_DELETE_ARRAY(p) { if((p) != nullptr){ delete[] (p); (p) = nullptr; } }
#endif

#ifndef PF_SAFE_RELEASE
#define PF_SAFE_RELEASE(p)      { if((p) != nullptr){ (p)->Release(); (p) = nullptr; } }
#endif

#ifndef PF_ARRAY_LENGTH
#define PF_ARRAY_LENGTH(array) (sizeof((array)) / sizeof((array)[0]))
#endif

#ifndef PF_MACRO_STRING
	#define PF_MACRO_STRING_(s) #s
	#define PF_MACRO_STRING(s) PF_MACRO_STRING_(s)
#endif

#ifndef PF_MACRO_STRING_W
	#define PF_MACRO_STRING_W_(s) L##s
	#define PF_MACRO_STRING_W(s) PF_MACRO_STRING_W_(s)
#endif

#ifndef _W
	#define __W(s) L##s
	#define _W(s) __W(s)
#endif
