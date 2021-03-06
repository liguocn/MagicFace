//
//  AppDelegate.m
//  MagicFace
//
//  Created by silence on 5/10/14.
//  Copyright (c) 2014 Magic3D. All rights reserved.
//

#import "AppDelegate.h"
#import "AppManager.h"
#import "MainApp.h"
#import "Face2DApp.h"

@implementation AppDelegate

- (void)SetHomeFrame
{
    [self.window setFrame:NSMakeRect(448, 456, 480, 400) display:YES];
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    // Insert code here to initialize your application
    [self SetHomeFrame];
    self.mMainView = self.window.contentView;
    [[AppManager GetAppManager] EnterApp:[[MainApp alloc] init] :@"MainApp" ];
}

- (IBAction)BackHome:(id)sender
{
    [self SetHomeFrame];
    self.window.contentView = self.mMainView;
    [[AppManager GetAppManager] SwitchCurrentApp:@"MainApp"];
}

- (IBAction)EnterFace2DApp:(id)sender
{
    if (self.mFace2DController == nil)
    {
        self.mFace2DController = [[Face2DController alloc] init];
    }
    //[self.window setMaxSize:NSMakeSize(1024, 768)];
    [self.window setFrame:NSMakeRect(448, 456, 1024, 520) display:YES];
    self.window.contentView = self.mFace2DController.view;
    if (![[AppManager GetAppManager] SwitchCurrentApp:@"Face2DApp"])
    {
        [[AppManager GetAppManager] EnterApp:[[Face2DApp alloc] init] :@"Face2DApp"];
    }
}

@end
