//
//  Face2DController.m
//  MagicFace
//
//  Created by silence on 5/12/14.
//  Copyright (c) 2014 Magic3D. All rights reserved.
//

#import "Face2DController.h"
#import "AppManager.h"
#import "Face2DApp.h"

@interface Face2DController ()

@end

@implementation Face2DController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Initialization code here.
    }
    return self;
}

- (NSPoint)TransferToLeftImagePositon:(NSPoint)mousePos
{
    NSPoint imageViewPos = [self.mLeftImageView frame].origin;
    NSSize imageViewSize = [self.mLeftImageView frame].size;
    NSPoint posInImg = NSMakePoint(mousePos.x - imageViewPos.x - self.mLeftImageRect.origin.x, imageViewSize.height + imageViewPos.y - self.mLeftImageRect.origin.y - mousePos.y);
    return posInImg;
}


- (IBAction)LoadImage:(id)sender
{
    self.mLeftImageRect = [self.mLeftImageView.cell drawingRectForBounds:self.mLeftImageView.bounds];
    NSOpenPanel *filePanel = [NSOpenPanel openPanel];
    if ([filePanel runModal] == NSOKButton)
    {
        NSString *filePath = [[[filePanel URLs] objectAtIndex:0] path];
        Face2DApp* f2dApp = (Face2DApp*)[[AppManager GetAppManager] GetApp:@"Face2DApp"];
        NSImage* img = [f2dApp LoadImage:filePath];
        //NSImage* img = [[NSImage alloc] initWithContentsOfFile:filePath];
        //NSImage* img = [[NSImage alloc] initWithContentsOfURL:[[filePanel URLs] objectAtIndex:0]];
        NSBitmapImageRep* imgRep = [[img representations] objectAtIndex:0];
        NSSize imgSize = [imgRep size];
        NSLog(@"image size: %f, %f\n", imgSize.width, imgSize.height);
        [self.mLeftImageView setImage:img];
        [self.mLeftImageView setImageAlignment:NSImageAlignTopLeft];
    }
}

- (IBAction)EditFeaturePoints:(id)sender
{
    
}

- (IBAction)DeformFeature:(id)sender
{
    
}

- (IBAction)DeformColor:(id)sender
{
    
}

- (IBAction)LoadRefImage:(id)sender
{
    
}

- (IBAction)CalRefFeature:(id)sender
{
    
}

- (IBAction)CalRefColor:(id)sender
{
    
}

@end
