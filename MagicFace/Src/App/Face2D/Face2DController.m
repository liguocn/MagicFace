//
//  Face2DController.m
//  MagicFace
//
//  Created by silence on 5/12/14.
//  Copyright (c) 2014 Magic3D. All rights reserved.
//

#import "Face2DController.h"

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
