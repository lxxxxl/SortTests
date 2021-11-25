#include "MenuScene.h"
#include "FieldScene.h"

Scene* MenuScene::createScene()
{
    return MenuScene::create();
}

// on "init" you need to initialize your instance
bool MenuScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    /////////////////////////////
    // 2. create menu items
    auto item1 = MenuItemFont::create("Bubble Sort", [&](Ref* sender){
        displayFieldScene(0);
    });
    auto item2 = MenuItemFont::create("Comb Sort", [&](Ref* sender){
        displayFieldScene(1);
    });

    //////////////////////////////
    // 3. create menu
    auto menu = Menu::create(item1, item2, nullptr);
    menu->alignItemsVertically();
    this->addChild(menu, 1);

    return true;
}

void MenuScene::displayFieldScene(int algo)
{
    auto scene = FieldScene::create();
    scene->setAlgo(algo);
    scene->startSorting();
    Director::getInstance()->replaceScene(scene);
}
