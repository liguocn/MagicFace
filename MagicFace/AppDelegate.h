//
//  AppDelegate.h
//  MagicFace
//
//  Created by silence on 5/10/14.
//  Copyright (c) 2014 Magic3D. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "Face2DController.h"

@interface AppDelegate : NSObject <NSApplicationDelegate>

@property (assign) IBOutlet NSWindow *window;
@property (retain) NSView* mpMainView;

@property (retain) IBOutlet Face2DController *mpFace2DController;

- (IBAction)BackHome:(id)sender;
- (IBAction)EnterFace2DApp:(id)sender;

@end
