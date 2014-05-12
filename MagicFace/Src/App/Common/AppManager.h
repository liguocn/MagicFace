//
//  AppManager.h
//  MagicWorld
//
//  Created by silence on 4/12/14.
//  Copyright (c) 2014 Magic3D. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AppBase.h"

@interface AppManager : NSObject

@property (retain) NSMutableDictionary *mpAppDic;
@property (retain) AppBase* mpCurrentApp;

+ (AppManager*)GetAppManager; //This is not a singleton!

- (void)EnterApp:(AppBase*)pApp :(NSString*)appName;
- (BOOL)SwitchCurrentApp:(NSString*)appName;
- (AppBase*)GetApp:(NSString*)appName;


@end
