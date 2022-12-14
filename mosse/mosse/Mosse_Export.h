// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:	MosseTracker
// Author:	Anton Egorov
// Warning:	Exported functions are NOT thread-safe, therefore
//			the control program must ensure thread safety.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


#ifndef MOSSEEXPORTS_H
#define MOSSEEXPORTS_H

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Export defines
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#define MOSSE_API extern "C"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Exported functions declaration
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


// Initializes the Mosse Tracker by the given image. Returns ID of the new tracker
MOSSE_API int Mosse_Init(
	const unsigned char *pScan0,		// [in] Pointer to the first element of the image
	int pStride,						// [in] Image stride
	int pX, int pY, int pW, int pH,		// [in] Initial rect
	float pLearnRate = 0.08f			// [in] The Mosse Tracker learning rate. Read the paper for more information
	);


// Processes the given image and moves the given rectangle to the new position
MOSSE_API void Mosse_OnFrame(
	int pTrackerId,						// [in]  ID of the Mosse Tracker to process the given image with
	const unsigned char *pScan0,		// [in]  Pointer to the first element of the image
	int pStride,						// [in]  Image stride
	int &pX, int &pY, int &pW, int &pH	// [out] Rectangle of interest to track object in, returns the new object position found by the Mosse Tracker
	);


// Forces the Mosse Tracker with the given ID to accumulate the new image part to the filter. Tracking isn't performed
MOSSE_API void Mosse_Train(
	int pTrackerId,						// [in]  ID of the Mosse Tracker to process the given image with
	const unsigned char *pScan0,		// [in]  Pointer to the first element of the image
	int pStride,						// [in]  Image stride
	int pX, int pY, int pW, int pH		// [out] Rectangle of interest to track object in, returns the new object position found by the Mosse Tracker
	);


// Disposes the Mosse Tracker with the given ID and releases all the memory associated with it
MOSSE_API void Mosse_Dispose(int pTrackerId);


// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Exported debug functions declaration
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


// Returns a pointer to the first element of the response matrix (real part) of the Tracker with the given ID.
// No thread safety, no null-checks
MOSSE_API float * Mosse_GetResponse(int pTrackerId);


// Returns a pointer to the first element of the filter matrix (real part) of the Tracker with the given ID.
// No thread safety, no null-checks
MOSSE_API float * Mosse_GetFilter(int pTrackerId);


#endif // MOSSEEXPORTS_H
