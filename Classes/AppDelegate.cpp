// AppDelegate.cpp (Updated - NO_BORDER, resolution-safe)

#include "AppDelegate.h"
#include "MenuScene.h"

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#endif

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(1280, 720);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate() {}
AppDelegate::~AppDelegate()
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#endif
}

void AppDelegate::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs = {8,8,8,8,24,8,0};
    GLView::setGLContextAttrs(glContextAttrs);
}

static int register_all_packages()
{
    return 0;
}

bool AppDelegate::applicationDidFinishLaunching() {
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();

    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("MyMemoryGame", cocos2d::Rect(0,0,designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("MyMemoryGame");
#endif
        director->setOpenGLView(glview);
    }

    director->setDisplayStats(false);
    director->setAnimationInterval(1.0f/60.0f);

    // Use NO_BORDER to avoid non-uniform scaling (preserves aspect ratio; may crop edges)
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);

    // contentScaleFactor: choose closest resource scale
    auto frameSize = glview->getFrameSize();
    float scaleFactor = 1.0f;
    if (frameSize.height > mediumResolutionSize.height) {
        scaleFactor = std::min(largeResolutionSize.height/designResolutionSize.height,
                               largeResolutionSize.width/designResolutionSize.width);
    } else if (frameSize.height > smallResolutionSize.height) {
        scaleFactor = std::min(mediumResolutionSize.height/designResolutionSize.height,
                               mediumResolutionSize.width/designResolutionSize.width);
    } else {
        scaleFactor = std::min(smallResolutionSize.height/designResolutionSize.height,
                               smallResolutionSize.width/designResolutionSize.width);
    }
    director->setContentScaleFactor(scaleFactor);

    register_all_packages();

    auto scene = MenuScene::createScene();
    director->runWithScene(scene);

    return true;
}

void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#endif
}

void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#endif
}
