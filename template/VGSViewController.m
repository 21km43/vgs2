/* (C)2014, SUZUKI PLAN.
 *----------------------------------------------------------------------------
 * Description: GameDaddy - emulator
 *    Platform: iOS
 *      Author: Yoji Suzuki (SUZUKI PLAN)
 *        Date: 15-Mar-2014
 * FileVersion: 1.01
 *----------------------------------------------------------------------------
 */
#import <iAd/iAd.h>
#import "VGSViewController.h"
#import "VGSScreenView.h"
#import "VGSView.h"
#import "VGSBarView.h"
#import "vgs2.h"

extern int isIphone5;

bool bannerIsVisible=NO;
int vgsint_init(const char* bin);
void vgsint_setdir(const char* dir);

@interface VGSViewController ()

@end

@implementation VGSViewController

- (void)viewDidLoad
{
    [super viewDidLoad];

    const float version = [[[UIDevice currentDevice] systemVersion] floatValue];
    NSLog(@"iOS version: %f", version);

    bool iPhone5=false;
    CGSize result = [[UIScreen mainScreen] bounds].size;
    CGFloat scale = [UIScreen mainScreen].scale;
    result = CGSizeMake(result.width * scale, result.height * scale);

    UIApplication *app = [UIApplication sharedApplication];
    app.statusBarHidden=YES;

    if (result.height == 1136.0) {
        iPhone5=true;
        isIphone5=1;
    } else {
        iPhone5=false;
        isIphone5=0;
    }
    self.view=[[VGSScreenView alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    // initialize VGS
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *docs_dir = [paths objectAtIndex:0];
    vgsint_setdir([docs_dir UTF8String]);
    NSString* fullPath;
    fullPath=[[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"romdata.bin"];
    if(vgsint_init([fullPath UTF8String])) {
        _exit(-1);
    }
    // set vram window
    if(iPhone5) {
        vram=[[VGSView alloc] initWithFrame:CGRectMake(0, 70, 320, 400)];
        [self.view addSubview:vram];
        UIImage *imageA= [UIImage imageNamed:@"panel1.png"];
        UIImageView *ivA= [[UIImageView alloc] initWithImage:imageA];
        ivA.frame=CGRectMake(0,20,320,50);
        [self.view addSubview:ivA];
        UIImage *imageB = [UIImage imageNamed:@"panel2.png"];
        UIImageView *ivB = [[UIImageView alloc] initWithImage:imageB];
        ivB.frame=CGRectMake(0,470,320,96);
        [self.view addSubview:ivB];
    } else {
        vram=[[VGSView alloc] initWithFrame:CGRectMake(0, 50, 320, 400)];
        [self.view addSubview:vram];
        UIImage *imageA= [UIImage imageNamed:@"panel1.png"];
        UIImageView *ivA= [[UIImageView alloc] initWithImage:imageA];
        ivA.frame=CGRectMake(0,0,320,50);
        [self.view addSubview:ivA];
        UIImage *imageB= [UIImage imageNamed:@"panel3.png"];
        UIImageView *ivB= [[UIImageView alloc] initWithImage:imageB];
        ivB.frame=CGRectMake(0,450,320,30);
        [self.view addSubview:ivB];
    }

    // create ad banner view
    ADBannerView *adView;
    adView = [[ADBannerView alloc] init];
    adView.frame = CGRectMake(0, -adView.frame.size.height, adView.frame.size.width, adView.frame.size.height);
    adView.delegate = self;
    adView.autoresizesSubviews = YES;
    adView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleBottomMargin;
    [self.view addSubview:adView];
    adView.alpha = 0.0;

    if(iPhone5) {
        // set status bar
        VGSBarView *bar=[[VGSBarView alloc] initWithFrame:CGRectMake(0,0,320,20)];
        [self.view addSubview:bar];
    }
}

- (void)bannerViewDidLoadAd:(ADBannerView *)banner
{
    if (!bannerIsVisible) {
        [UIView beginAnimations:@"animateAdBannerOn" context:NULL];
        [UIView setAnimationDuration:0.3];
        if(isIphone5) {
            banner.frame = CGRectOffset(banner.frame, 0, 20+CGRectGetHeight(banner.frame));
        } else {
            banner.frame = CGRectOffset(banner.frame, 0, CGRectGetHeight(banner.frame));
        }
        banner.alpha = 1.0;
        [UIView commitAnimations];
        bannerIsVisible = YES;
    }
}

- (void)bannerView:(ADBannerView *)banner didFailToReceiveAdWithError:(NSError *)error
{
    if (bannerIsVisible) {
        [UIView beginAnimations:@"animateAdBannerOff" context:NULL];
        [UIView setAnimationDuration:0.3];
        if(isIphone5) {
            banner.frame = CGRectOffset(banner.frame, 0, -(20+CGRectGetHeight(banner.frame)));
        } else {
            banner.frame = CGRectOffset(banner.frame, 0, -CGRectGetHeight(banner.frame));
        }
        banner.alpha = 0.0;
        [UIView commitAnimations];
        bannerIsVisible = NO;
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

- (BOOL)prefersStatusBarHidden
{
    return NO;
}

@end
