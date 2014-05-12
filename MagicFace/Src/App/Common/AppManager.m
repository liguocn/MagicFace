//
//  AppManager.m
//  MagicWorld
//
//  Created by silence on 4/12/14.
//  Copyright (c) 2014 Magic3D. All rights reserved.
//

#import "AppManager.h"

@implementation AppManager

+ (AppManager*)GetAppManager
{
    static AppManager* MagicAppManager = nil;
    static dispatch_once_t predicate;
    dispatch_once(&predicate, ^{
        MagicAppManager = [[self alloc] init];
    });
    
    return MagicAppManager;
}

- (void)EnterApp:(AppBase*)pApp :(NSString*)appName
{
    if (self.mpAppDic == nil)
    {
        self.mpAppDic = [[NSMutableDictionary alloc] init];
        [self.mpAppDic setObject:pApp forKey:appName];
        self.mpCurrentApp = pApp;
        [pApp Enter];
    }
    else if (self.mpCurrentApp != pApp)
    {
        AppBase* pFindObj = [self.mpAppDic objectForKey:appName];
        if (pFindObj == nil)
        {
            [self.mpAppDic setObject:pApp forKey:appName];
            pFindObj = pApp;
        }
        if (self.mpCurrentApp != nil)
        {
            [self.mpCurrentApp Exit];
        }
        self.mpCurrentApp = pFindObj;
        [pFindObj Enter];
    }
}

- (BOOL)SwitchCurrentApp:(NSString*)appName
{
    AppBase* pFindApp = [self.mpAppDic objectForKey:appName];
    if (pFindApp == nil)
    {
        return NO;
    }
    else if (self.mpCurrentApp == pFindApp)
    {
        return NO;
    }
    else
    {
        if (self.mpCurrentApp != nil)
        {
            [self.mpCurrentApp Exit];
        }
        self.mpCurrentApp = pFindApp;
        return [self.mpCurrentApp Enter];
    }
}

- (AppBase*)GetApp:(NSString*)appName
{
    AppBase* pFindApp = [self.mpAppDic objectForKey:appName];
    return pFindApp;
}

@end
