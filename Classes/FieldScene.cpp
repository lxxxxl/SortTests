#include "FieldScene.h"
#include "MenuScene.h"

Scene* FieldScene::createScene()
{
    return FieldScene::create();
}

// on "init" you need to initialize your instance
bool FieldScene::init()
{
    // 1. super init
    if ( !Scene::init() )
    {
        return false;
    }

    // 2. init tilemap
    _tileMap = new TMXTiledMap();
    _tileMap->initWithTMXFile("res/map.tmx");
    _background = _tileMap->getLayer("background");
    _objects = _tileMap->getLayer("objects");
    this->addChild(_tileMap, -1);


    // 3. create sprites
    for (int i = 0; i < 30; i++){
        Vec2 pos(i*SPRITE_SIZE, RandomHelper::random_int(1, 19) * SPRITE_SIZE);
        //_rocks->setTileGID(56, tile);
        auto sprite = spriteFromTileset(RandomHelper::random_int(5, 47));
        sprite->setPosition(pos);
        sprite->setTag(pos.y/SPRITE_SIZE);
        
        _objects->addChild(sprite);
        _sprites[i] = sprite;
    }

    return true;
}

void FieldScene::startSorting()
{
    switch (algo){
        case eaBubbleSort:
            bubbleSort();
            break;
        case eaCombSort:
            combSort();
            break;
        case eaShakerSort:
            combSort();
            break;
        default:
            break;
    }

    // delay after sorting end;
    TargetedAction *actionEndDelay = TargetedAction::create(this, DelayTime::create(3.0f));
    _actions.pushBack(actionEndDelay);
    // go back to main menu
    TargetedAction *actionBack = TargetedAction::create(this,  CallFunc::create(CC_CALLBACK_0(FieldScene::endSorting, this)));
    _actions.pushBack(actionBack);
    runAction(Sequence::create(_actions));
}

void FieldScene::endSorting()
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
    TargetedAction *action = TargetedAction::create(sprite, MoveTo::create(0.05f, Vec2(x*SPRITE_SIZE, y*SPRITE_SIZE)));
    _actions.pushBack(action);
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

void FieldScene::combSort()
{
    double factor = 1.2473309;
    int len = FIELD_WIDTH;
    int step = len / 2;
    while (step >= 1){
        for (int i = 0; i + step < len; i++){
            if (_sprites[i]->getTag() > _sprites[i+step]->getTag()){
                setNewCoords(_sprites[i], i+step, _sprites[i]->getTag());
                setNewCoords(_sprites[i+step], i, _sprites[i+step]->getTag());
                std::swap(_sprites[i], _sprites[i+step]);
            }
        }
        step /= factor;
    }
}

void FieldScene::shakerSort()
{
    int control = FIELD_WIDTH;
    int left  = 0;
    int right = FIELD_WIDTH;
    do {
        for (int i = left; i < right; i++) {
            if (_sprites[i]->getTag() > _sprites[i+1]->getTag()) {
                setNewCoords(_sprites[i], i+1, _sprites[i]->getTag());
                setNewCoords(_sprites[i+1], i, _sprites[i+1]->getTag());
                std::swap(_sprites[i], _sprites[i+1]);
                control = i;
            }
        }
        right = control;
        for (int i = right; i > left; i--) {
            if (_sprites[i]->getTag() > _sprites[i-1]->getTag()) {
                setNewCoords(_sprites[i], i-1, _sprites[i]->getTag());
                setNewCoords(_sprites[i-1], i, _sprites[i-1]->getTag());
                std::swap(_sprites[i], _sprites[i-1]);
                control = i;
            }
        }
        left = control;
    } while (left < right);
}
