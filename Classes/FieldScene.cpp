#include "FieldScene.h"
#include "MenuScene.h"

Scene* FieldScene::createScene()
{
    return FieldScene::create();
}

// on "init" you need to initialize your instance
bool FieldScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    _tileMap = new TMXTiledMap();
    _tileMap->initWithTMXFile("res/map.tmx");
    _background = _tileMap->getLayer("background");
    _objects = _tileMap->getLayer("objects");

    
    this->addChild(_tileMap, -1);

    for (int i = 0; i < 30; i++){
        Vec2 pos(i*SPRITE_SIZE, RandomHelper::random_int(1, 19) * SPRITE_SIZE);
        //_rocks->setTileGID(56, tile);
        auto sprite = spriteFromTileset(RandomHelper::random_int(5, 47));
        sprite->setPosition(pos);
        sprite->setTag(pos.y/SPRITE_SIZE);
        
        _objects->addChild(sprite);
        _sprites[i] = sprite;
    }

    //http://indivour.blogspot.com/2016/02/cocos2d-x-v3x-calling-functions-after.html
    float delay = 3.0f;
    this->scheduleOnce(CC_SCHEDULE_SELECTOR(FieldScene::startSorting), delay);

    return true;
}

void FieldScene::startSorting(float)
{
    switch (algo){
        case eaBubbleSort:
            bubbleSort();
            break;
        default:
            break;
    }

    float delay = 3.0f;
    this->scheduleOnce(CC_SCHEDULE_SELECTOR(FieldScene::endSorting), delay);
}

void FieldScene::endSorting(float)
{
    auto scene = MenuScene::create();
    Director::getInstance()->replaceScene(scene);
}

void FieldScene::setAlgo(int a)
{
    algo = a;
}

int FieldScene::getAlgo() const
{
    return algo;
}

void FieldScene::setNewCoords(Sprite *sprite, int x, int y)
{
    auto moveTo = MoveTo::create(0.4f, Vec2(x*SPRITE_SIZE, y*SPRITE_SIZE));
    sprite->setPosition(Vec2(x*SPRITE_SIZE, y*SPRITE_SIZE));
    //sprite->runAction(moveTo);
}

Sprite *FieldScene::spriteFromTileset(int gid)
{
    int y = gid / SPRITES_PER_LINE;
    int x = gid % SPRITES_PER_LINE;
    auto sprite = Sprite::create("res/toen.png", Rect(x*SPRITE_SIZE,y*SPRITE_SIZE,SPRITE_SIZE,SPRITE_SIZE));
    sprite->setAnchorPoint(Vec2(0,0));
    return sprite;
    
}


void FieldScene::bubbleSort()
{
    bool sorted = false;
    while (!sorted){
        sorted = true;
        for (int i = 0; i < FIELD_WIDTH-1; i++){
            if (_sprites[i]->getTag() > _sprites[i+1]->getTag()){
                setNewCoords(_sprites[i], i+1, _sprites[i]->getTag());
                setNewCoords(_sprites[i+1], i, _sprites[i+1]->getTag());
                std::swap(_sprites[i], _sprites[i+1]);
                sorted = false;
            }
        }
    }
}