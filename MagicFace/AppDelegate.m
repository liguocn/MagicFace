//
//  AppDelegate.m
//  MagicFace
//
//  Created by silence on 5/10/14.
//  Copyright (c) 2014 Magic3D. All rights reserved.
//

#import "AppDelegate.h"

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    // Insert code here to initialize your application
}

- (IBAction)BackHome:(id)sender
{
    self.window.contentView = self.mpMainView;
}

- (IBAction)EnterFace2DApp:(id)sender
{
    if (self.mpFace2DController == nil)
    {
        self.mpFace2DController = [[Face2DController alloc] init];
    }
    //[self.window setMaxSize:NSMakeSize(1024, 768)];
    [self.window setFrame:NSMakeRect(448, 156, 1024, 768) display:YES];
    self.window.contentView = self.mpFace2DController.mpMainView;
}

@end
