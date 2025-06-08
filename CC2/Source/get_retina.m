//
// Created by Marek Ratajczak on 05/30/25.
//

#ifdef __APPLE__
#include <objc/objc.h>
#include <objc/runtime.h>
#include <objc/message.h>

#ifdef __cplusplus
extern "C" {
#endif
// Function to get the backingScaleFactor of the main screen
double getBackingScaleFactor() {
    // Get the NSScreen class
    Class nsScreenClass = objc_getClass("NSScreen");
    if (!nsScreenClass) {
        return 1.0; // Fallback if NSScreen class not found
    }

    // Get the mainScreen selector
    SEL mainScreenSelector = sel_registerName("mainScreen");
    if (!mainScreenSelector) {
        return 1.0;
    }

    // Call [NSScreen mainScreen]
    id mainScreen = ((id (*)(Class, SEL))objc_msgSend)(nsScreenClass, mainScreenSelector);
    if (!mainScreen) {
        return 1.0;
    }

    // Get the backingScaleFactor selector
    SEL backingScaleFactorSelector = sel_registerName("backingScaleFactor");
    if (!backingScaleFactorSelector) {
        return 1.0;
    }

    // Call [mainScreen backingScaleFactor]
    double scaleFactor = ((double (*)(id, SEL))objc_msgSend)(mainScreen, backingScaleFactorSelector);

    return scaleFactor;
}

#ifdef __cplusplus
}
#endif

#endif