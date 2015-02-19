/*
 * Copyright (C) 2007-2008 Alacatia Labs
 * 
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 * 
 * Joe Ranieri joe@alacatia.com
 *
 */

#pragma once
#include "CGSConnection.h"
#include "CGSRegion.h"

typedef int CGSWindowID;
typedef int CGSAnimationObj;
typedef struct { CGPoint localPoint; CGPoint globalPoint; } CGSWarpPoint;

typedef enum {
	kCGSSharingNone,
	kCGSSharingReadOnly,
	kCGSSharingReadWrite
} CGSSharingState;

typedef enum {
	kCGSOrderBelow = -1,
	kCGSOrderOut, /* hides the window */
	kCGSOrderAbove,
	kCGSOrderIn /* shows the window */
} CGSWindowOrderingMode;

typedef enum {
   kCGSBackingNonRetianed,
   kCGSBackingRetained,
   kCGSBackingBuffered,
} CGSBackingType;


CG_EXTERN_C_BEGIN

/*! Switches to the next (or previous) window in the global list. */
CG_EXTERN CGError CGSCycleWindows(CGSConnectionID cid, CGSWindowOrderingMode order);

/*! Gets and sets the desktop window. Not sure what happens if more than one process sets the desktop window. */
CG_EXTERN CGError CGSDesktopWindow(CGSConnectionID cid, CGSWindowID *outWID);
CG_EXTERN CGError CGSSetDesktopWindow(CGSConnectionID cid, CGSWindowID wid);

/*! Sets the window's title. Internally this simply calls `CGSSetWindowProperty(cid, wid, kCGSWindowTitle, title)`. */
CG_EXTERN CGError CGSSetWindowTitle(CGSConnectionID cid, CGSWindowID wid, CFStringRef title);

/*! Gets and sets a property for a window. */
CG_EXTERN CGError CGSGetWindowProperty(CGSConnectionID cid, CGSWindowID wid, CFStringRef key, CFTypeRef *outValue);
CG_EXTERN CGError CGSSetWindowProperty(CGSConnectionID cid, CGSWindowID wid, CFStringRef key, CFTypeRef value);

/*! Gets and sets whether a window is opaque. */
CG_EXTERN CGError CGSGetWindowOpacity(CGSConnectionID cid, CGSWindowID wid, bool *isOpaque);
CG_EXTERN CGError CGSSetWindowOpacity(CGSConnectionID cid, CGSWindowID wid, bool isOpaque);

/*! Gets and sets the window's transparency. */
CG_EXTERN CGError CGSGetWindowAlpha(CGSConnectionID cid, CGSWindowID wid, float *outAlpha);
CG_EXTERN CGError CGSSetWindowAlpha(CGSConnectionID cid, CGSWindowID wid, float alpha);

/*! Gets and sets the window's transform. 
 
	Severe restrictions are placed on transformation:
 - Transformation Matrices may only include a singular transform.
 - Transformations involving scale may not scale upwards past the window's frame.
 - Transformations involving rotation must be followed by translation or the window will fall offscreen.
 */
CG_EXTERN CGError CGSGetWindowTransform(CGSConnectionID ciw, CGSWindowID wid, const CGAffineTransform *outTransform);
CG_EXTERN CGError CGSSetWindowTransform(CGSConnectionID ciw, CGSWindowID wid, CGAffineTransform transform);

/*! Gets and sets the window's transform in place. 
	
	Severe restrictions are placed on transformation:
 - Transformation Matrices may only include a singular transform.
 - Transformations involving scale may not scale upwards past the window's frame.
 - Transformations involving rotation must be followed by translation or the window will fall offscreen.
 */
CG_EXTERN CGError CGSGetWindowTransformAtPlacement(CGSConnectionID ciw, CGSWindowID wid, const CGAffineTransform *outTransform);
CG_EXTERN CGError CGSSetWindowTransformAtPlacement(CGSConnectionID ciw, CGSWindowID wid, CGAffineTransform transform);

/*! Sets the alpha of a group of windows over a period of time. Note that `duration` is in seconds. */
CG_EXTERN CGError CGSSetWindowListAlpha(CGSConnectionID cid, const CGSWindowID *widList, int widCount, float alpha, float duration);

/*! Gets and sets the `CGConnectionID` that owns this window. Only the owner can change most properties of the window. */
CG_EXTERN CGError CGSGetWindowOwner(CGSConnectionID cid, CGSWindowID wid, CGSConnectionID *outOwner);
CG_EXTERN CGError CGSSetWindowOwner(CGSConnectionID cid, CGSWindowID wid, CGSConnectionID owner);

/*! Sets the background color of the window. */
CG_EXTERN CGError CGSSetWindowAutofillColor(CGSConnectionID cid, CGSWindowID wid, float red, float green, float blue);

/*! Locks a window to the cursor, so that whenever the cursor moves, the window moves with it. There doesn't seem to be a way to unlock the window from the cursor. */
CG_EXTERN CGError CGSLockWindowToCursor(CGSConnectionID cid, CGSWindowID wid, float offsetLeft, float offsetTop);

/*! Sets the warp for the window. The mesh maps a local (window) point to a point on screen. */
CG_EXTERN CGError CGSSetWindowWarp(CGSConnectionID cid, CGSWindowID wid, int warpWidth, int warpHeight, const CGSWarpPoint *warp);

/*! Gets or sets whether the window server should auto-fill the window's background. */
CG_EXTERN CGError CGSGetWindowAutofill(CGSConnectionID cid, CGSWindowID wid, bool *outShouldAutoFill);
CG_EXTERN CGError CGSSetWindowAutofill(CGSConnectionID cid, CGSWindowID wid, bool shouldAutoFill);

/*! Gets the screen rect for a window. */
CG_EXTERN CGError CGSGetScreenRectForWindow(CGSConnectionID cid, CGSWindowID wid, CGRect *outRect);

/*! Gets and sets the window level for a window. */
CG_EXTERN CGError CGSGetWindowLevel(CGSConnectionID cid, CGSWindowID wid, CGWindowLevel *outLevel);
CG_EXTERN CGError CGSSetWindowLevel(CGSConnectionID cid, CGSWindowID wid, CGWindowLevel level);

/*! Gets and sets the sharing state. This determines the level of access other applications have over this window. */
CG_EXTERN CGError CGSGetWindowSharingState(CGSConnectionID cid, CGSWindowID wid, CGSSharingState *outState);
CG_EXTERN CGError CGSSetWindowSharingState(CGSConnectionID cid, CGSWindowID wid, CGSSharingState state);

/*! Sets whether this window is ignored in the global window cycle (Control-F4 by default). There is no Get version? */
CG_EXTERN CGError CGSSetIgnoresCycle(CGSConnectionID cid, CGSWindowID wid, bool ignoresCycle);

/*! Creates a graphics context for the window. 
 
 Acceptable keys options:
 
 - CGWindowContextShouldUseCA : CFBooleanRef
 */
CG_EXTERN CGContextRef CGWindowContextCreate(CGSConnectionID cid, CGSWindowID wid, CFDictionaryRef options);

/*! Sets the order of a window */
CG_EXTERN CGError CGSOrderWindow(CGSConnectionID cid, CGSWindowID wid, CGSWindowOrderingMode mode, CGSWindowID relativeToWID);

/*! Sets the origin (top-left) of a window */
CG_EXTERN CGError CGSMoveWindow(CGSConnectionID cid, CGSWindowID wid, const CGPoint *origin);

/*! Sets the origin (top-left) of a window relative to another window's origin. */
CG_EXTERN CGError CGSSetWindowOriginRelativeToWindow(CGSConnectionID cid, CGSWindowID wid, CGSWindowID relativeToWID, float offsetX, float offsetY);

/*! Sets the frame and position of a window.  Updates are grouped for the sake of animation. */
CG_EXTERN CGError CGSMoveWindowWithGroup(CGSConnectionID cid, CGSWindowID wid, CGRect *newFrame);

/* Flushes a window's buffer to the screen. */
CG_EXTERN CGError CGSFlushWindow(CGSConnectionID cid, CGSWindowID wid, CGSRegionObj flushRegion);


#pragma mark shadows

/*! Gets and sets the shadow information for a window. Values for `flags` are unknown. */
CG_EXTERN CGError CGSSetWindowShadowAndRimParameters(CGSConnectionID cid, CGSWindowID wid, float standardDeviation, float density, int offsetX, int offsetY, int flags);
CG_EXTERN CGError CGSGetWindowShadowAndRimParameters(CGSConnectionID cid, CGSWindowID wid, float *outStandardDeviation, float *outDensity, int *outOffsetX, int *outOffsetY, int *outFlags);

/*! Sets the shadow information for a window. Simply calls through to `CGSSetWindowShadowAndRimParameters` passing 1 for `flags`. */
CG_EXTERN CGError CGSSetWindowShadowParameters(CGSConnectionID cid, CGSWindowID wid, float standardDeviation, float density, int offsetX, int offsetY);

/*! Invalidates a window's shadow. */
CG_EXTERN CGError CGSInvalidateWindowShadow(CGSConnectionID cid, CGSWindowID wid);

/*!  
 Sets a window's shadow properties.
 
 Acceptable keys:
 
 - com.apple.WindowShadowDensity		- (0.0 - 1.0) Opacity of the window's shadow.
 - com.apple.WindowShadowRadius			- The radius of the shadow around the window's corners.
 - com.apple.WindowShadowVerticalOffset	- Vertical offset of the shadow.
 - com.apple.WindowShadowRimDensity		- (0.0 - 1.0) Opacity of the black rim around the window.
 - com.apple.WindowShadowRimStyleHard	- Sets a hard black rim around the window.
 */
CG_EXTERN CGError CGSWindowSetShadowProperties(CGSWindowID wid, CFDictionaryRef properties);


#pragma mark window lists

/*! Gets the number of windows the `targetCID` owns. */
CG_EXTERN CGError CGSGetWindowCount(CGSConnectionID cid, CGSConnectionID targetCID, int *outCount);

/*! Gets a list of windows owned by `targetCID`. */
CG_EXTERN CGError CGSGetWindowList(CGSConnectionID cid, CGSConnectionID targetCID, int count, CGSWindowID *list, int *outCount);

/*! Gets the number of windows owned by `targetCID` that are on screen. */
CG_EXTERN CGError CGSGetOnScreenWindowCount(CGSConnectionID cid, CGSConnectionID targetCID, int *outCount);

/*! Gets a list of windows oned by `targetCID` that are on screen. */
CG_EXTERN CGError CGSGetOnScreenWindowList(CGSConnectionID cid, CGSConnectionID targetCID, int count, CGSWindowID *list, int *outCount);


#pragma mark window management

/*! Creates a new CGSWindow. The real window top/left is the sum of the region's top/left and the top/left parameters. */
CG_EXTERN CGError CGSNewWindow(CGSConnectionID cid, CGSBackingType backingType, float left, float top, CGSRegionObj region, CGSWindowID *outWID);

/*! Creates a new CGSWindow. The real window top/left is the sum of the region's top/left and the top/left parameters. */
CG_EXTERN CGError CGSNewWindowWithOpaqueShape(CGSConnectionID cid, CGSBackingType backingType, float left, float top, CGSRegionObj region, CGSRegionObj opaqueShape, int unknown, const int *tags, int tagSize, CGSWindowID *outWID);

/*! Releases a CGSWindow. */
CG_EXTERN CGError CGSReleaseWindow(CGSConnectionID cid, CGSWindowID wid);


#pragma mark animations

/*! Creates a Dock-style genie animation that goes from `wid` to `destinationWID`. */
CG_EXTERN CGError CGSCreateGenieWindowAnimation(CGSConnectionID cid, CGSWindowID wid, CGSWindowID destinationWID, CGSAnimationObj *outAnimation);

/*! Creates a sheet animation that's used when the parent window is brushed metal. Oddly enough, seems to be the only one used, even if the parent window isn't metal. */
CG_EXTERN CGError CGSCreateMetalSheetWindowAnimationWithParent(CGSConnectionID cid, CGSWindowID wid, CGSWindowID parentWID, CGSAnimationObj *outAnimation);

/*! Sets the progress of an animation. */
CG_EXTERN CGError CGSSetWindowAnimationProgress(CGSAnimationObj animation, float progress);

/*! DOCUMENTATION PENDING */
CG_EXTERN CGError CGSWindowAnimationChangeLevel(CGSAnimationObj animation, CGWindowLevel level);

/*! DOCUMENTATION PENDING */
CG_EXTERN CGError CGSWindowAnimationSetParent(CGSAnimationObj animation, CGSWindowID parent) AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;

/*! Releases a window animation. */
CG_EXTERN CGError CGSReleaseWindowAnimation(CGSAnimationObj animation);


#pragma mark window accelleration

/*! Gets the state of accelleration for the window. */
CG_EXTERN CGError CGSWindowIsAccelerated(CGSConnectionID cid, CGSWindowID wid, bool *outIsAccelerated);

/*! Gets and sets if this window can be accellerated. I don't know if playing with this is safe. */
CG_EXTERN CGError CGSWindowCanAccelerate(CGSConnectionID cid, CGSWindowID wid, bool *outCanAccelerate);
CG_EXTERN CGError CGSWindowSetCanAccelerate(CGSConnectionID cid, CGSWindowID wid, bool canAccelerate);


#pragma mark system status items

/*! Registers or unregisters a window as a global status item (see `NSStatusItem`, `NSMenuExtra`). Once a window is registered, the window server takes care of placing it in the apropriate location. */
CG_EXTERN CGError CGSRegisterWindowWithSystemStatusBar(CGSConnectionID cid, CGSWindowID wid, int priority);
CG_EXTERN CGError CGSUnregisterWindowWithSystemStatusBar(CGSConnectionID cid, CGSWindowID wid);

/*! Rearranges items in the system status bar. You should call this after registering or unregistering a status item or changing the window's width. */
CG_EXTERN CGError CGSAdjustSystemStatusBarWindows(CGSConnectionID cid);

#pragma mark window tags

typedef enum {
	CGSTagNone			= 0,		// No tags
	CGSTagExposeFade	= 0x0002,	// Fade out when Expose activates.
	CGSTagNoShadow		= 0x0008,	// No window shadow.
	CGSTagTransparent   = 0x0200,   // Transparent to mouse clicks.
	CGSTagSticky		= 0x0800,	// Appears on all workspaces.
} CGSWindowTag;

/*! Get the given tags for a window.  `thirtyTwoOrSixtyFour` must be either 32 or 64 for some reason... */
CG_EXTERN CGError CGSGetWindowTags(CGSConnectionID cid, CGSWindowID wid, CGSWindowTag *tags, int thirtyTwoOrSixtyFour);

/*! Set the given tags for a window.  `thirtyTwoOrSixtyFour` must be either 32 or 64 for some reason... */
CG_EXTERN CGError CGSSetWindowTags(CGSConnectionID cid, CGSWindowID wid, CGSWindowTag *tags, int thirtyTwoOrSixtyFour);

/*! Clear the given tags for a window.  `thirtyTwoOrSixtyFour` must be either 32 or 64 for some reason... */
CG_EXTERN CGError CGSClearWindowTags(CGSConnectionID cid, CGSWindowID wid, CGSWindowTag *tags, int thirtyTwoOrSixtyFour);

CG_EXTERN_C_END
