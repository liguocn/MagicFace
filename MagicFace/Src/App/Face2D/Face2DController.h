//
//  Face2DController.h
//  MagicFace
//
//  Created by silence on 5/12/14.
//  Copyright (c) 2014 Magic3D. All rights reserved.
//

#import <Cocoa/Cocoa.h>
//#import "Face2DView.h"

@interface Face2DController : NSViewController
enum MouseMode
{
    MM_View,
    MM_Move_Feature,
    MM_Auto_Align_Feature
};

//@property (retain) IBOutlet Face2DView* mMainView;
@property (retain) IBOutlet NSImageView* mLeftImageView;
@property (retain) IBOutlet NSImageView* mMidImageView;
@property (retain) IBOutlet NSImageView* mRightImageView;
@property NSRect mLeftImageRect;
@property enum MouseMode mMouseMode;
@property BOOL mIsFeaturePointSelected;

- (NSPoint)TransferToLeftImagePositon:(NSPoint)mousePos;
- (IBAction)LoadImage:(id)sender;
- (IBAction)EditFeaturePoints:(id)sender;
- (IBAction)DeformFeature:(id)sender;
- (IBAction)DeformColor:(id)sender;
- (IBAction)LoadRefImage:(id)sender;
- (IBAction)CalRefFeature:(id)sender;
- (IBAction)CalRefColor:(id)sender;

@end
