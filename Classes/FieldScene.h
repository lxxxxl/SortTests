#ifndef __FIELD_SCENE_H__
#define __FIELD_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

#define SPRITES_PER_LINE    7
#define SPRITE_SIZE         16
#define FIELD_WIDTH         30
#define FIELD_HEIGHT        20

class FieldScene : public Scene
{
public:
    enum eAlgo {
        eaBubbleSort,
        eaCombSort,
        eaShakerSort,
        eaRadixSort
    };

    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(FieldScene);
    Sprite *spriteFromTileset(int gid);
    // move sprite to specified location.
    // x and y are in tileno, NOT screen coords
    void setNewCoords(Sprite *sprite, int x, int y);
    // callback for start sorting procedure
    void startSorting();
    // callback for end sorting procedure
    void endSorting();

    void bubbleSort();
    void combSort();
    void shakerSort();
    void radixSort();


    CC_PROPERTY(int, algo, Algo);



private:
    TMXTiledMap *_tileMap;
    TMXLayer *_background;
    TMXLayer *_objects;
    Sprite *_sprites[FIELD_WIDTH];
    Vector< FiniteTimeAction * > _actions;

    typedef void (FieldScene::*SortFunction)();
    std::vector<SortFunction> _algos;
};

#endif // __FIELD_SCENE_H__
