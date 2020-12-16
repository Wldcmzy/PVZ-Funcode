/////////////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////
#ifndef _LESSON_X_H_
#define _LESSON_X_H_
//

#define LINE_LEFT -34       //草坪最左
#define LINE_TOP -26        //草坪最上
#define LINE_RIGHT 48       //草坪最右
#define LINE_BOTTOM 30      //草坪最下
#define CELL_WIDTH 9        //草坪格宽
#define CELL_HEIGHT 11.3    //草坪格高
#define OFFSET_WIDTH 4      //偏移：格子左侧到中心的距离
#define OFFSET_HEIGHT 8     //偏移：格子上方到中心的距离

#define ZOMBIE_SUM 75       //僵尸数量 不要大于99

#include <Windows.h>
#include <cstdlib>
#include <ctime>
class CardCSprite : public CSprite
{
private:
    int cd;
    int lastcreate;
    int cardIndex;
    int cost;
public:
    CardCSprite(const char *szName, const int index) : CSprite( szName )
    {
        lastcreate  =   0;
        cardIndex   =   index;
        switch(cardIndex)
        {
        case 0:
            cd      =   7;
            cost    =   50;
            break;
        case 2:
            cd      =   7;
            cost    =   100;
            break;
        case 4:
            cd      =   7;
            cost    =   200;
            break;
        case 1:
            cd      =   27;
            cost    =   50;
            break;
        case 3:
            cd      =   36;
            cost    =   150;
            break;
        }
    }
    int     getCost()                   { return cost; }
    int     getCD()                     { return cd; }
    int     getLastCreate()             { return lastcreate; }
    void    setLastCreate(int time)     { lastcreate    =   time; }
    void    setCD(int value)            { cd            =   value; }
};

class PeaCSprite : public CSprite
{
private :
    int turn;
public:
    PeaCSprite(const char *szName, const char *szCloneName, const int thetrun) : CSprite( szName, szCloneName )
    {
        turn    =   thetrun;
    }
    getTurn()               { return turn; }
};

class LifeCSprite : public CSprite
{
private:
    int             hp;
    int             cellX;
    int             cellY;
    int             cardIndex;
    int             lastShoot;
    int             creatTime;
    int             hasCap;
    int             isOverload;
    int             overloadTime;

public:
    //构造函数
    LifeCSprite( const char *szName, const char *szCloneName, const int X, const int Y, const int index ) : CSprite( szName, szCloneName )
    {
        cellX           =   X;
        cellY           =   Y;
        cardIndex       =   index;
        creatTime       =   clock()/CLOCKS_PER_SEC;
        lastShoot       =   creatTime;
        isOverload      =   0;
        switch(cardIndex)
        {
        case 1:
        case 3:
            hp          =   2400;
            break;
        case 0:
        case 2:
        case 4:
            hp          =   350;
            break;
        case 5:
            hp          =   1200;
            hasCap      =   0;
            break;
        case 6:
            hp          =   2000;
            hasCap      =   1;
            break;
        case 7:
            hp          =   3000;
            hasCap      =   1;
            break;
        case 101:
            hp          =1600;//这是第二形态的坚果墙
            break;
        case 102:
            hp          =800;//这是第三形态的坚果墙
            break;
        }
    };
    //~LifeCSprite()
    //{

    //}

    //get函数
    int             getHp()                         { return hp; }
    int             getCellX()                      { return cellX; }
    int             getCellY()                      { return cellY; }
    int             getCardIndex()                  { return cardIndex; }
    int             getCreateTime()                 { return creatTime; }
    int             getLastShoot()                  { return lastShoot; }
    int             getHasCap()                     { return hasCap; }
    int             getIsOverload()                 { return isOverload; }
    int             getOverloadTime()               { return overloadTime; }

    //set函数
    void            setHp( int toState )            { hp            =   toState; }
    void            setLastShoot(int value)         { lastShoot     =   value; }
    void            setHasCap(int value)            { hasCap        =   value; }
    void            setIsOverload(int value)        { isOverload    =   value; }
    void            setOverloadTime(int time)       { overloadTime  =   time; }
};



/////////////////////////////////////////////////////////////////////////////////
//
// 游戏总管类。负责处理游戏主循环、游戏初始化、结束等工作
class	CGameMain
{
private:
	int				m_iGameState;				// 游戏状态，0：结束或者等待开始；1：初始化；2：游戏进行中
	int             gameFocus;                  // 游戏焦点  1：开始  2：游戏  3：菜单 4：输
	int             gameLevel;                  // 游戏难度 0：简单 1：普通 2：老年人模式
	int             startTime;
    int             mouseCarry;                 // 鼠标状态，0：未持有物体；1：持有植物 2:铲子 3:僵尸 4:LBW得得得·啸
    int             filed;                      //1：失败 0：未失败
    int             cellHas_plant[9][5];       // 每个草格上的植物存在状态 不存在为负，大于等于0代表存在：存在植物在plants数组中的顺序，以0开头
    int             cardChoose;                 //玩家选择了哪个卡片，未选择为负数
    int             rowZombieNum[5];
    int             zombieRemain;                //伺机待发的僵尸数量
    int             zombieState;                //出僵尸的方式  1：前期 2：中期 ,3:后期 4：最后一波 5：死亡冲锋
    int             zombiePower;                //僵尸的整体强度,调用改属性的set函数后会对zombieList更改
    int             zombieList[5];              //僵尸列表，用于随机刷僵尸，1：普通僵尸2：路障僵尸3：铁桶僵尸，相同数字越多，该类型僵尸刷新概率越高
    int             zombieCreatWaitTime;        //两个僵尸刷新的时间间隔
    int             zombieCreatNum;             //一次刷新僵尸的数量
    int             zombieLastCreateTime;       //上次刷僵尸的时间
    int             hasZombie[ZOMBIE_SUM];      //这个zombie_body对应下标是否指向一个僵尸对象  是：1 不是：0
    int             sunLightNum;
    int             hasSunLight[10];
    int             sunLightClickIndex;
    int             lastSunLightCreatTime;
    int             mySunLight;
    int             sunLightCreatTimeSpan;      //阳光刷新时间间隔
    int             peaTurn;                    //豌豆序号
    int             musicState;                 //音乐 -1：代表无音乐 正数代表有音乐 详情见SCound*数组bgm
    int             lbwUsed;

public:
    CSprite         *chanziDong=new CSprite("chanzidong"),*chanziJing=new CSprite("chanzijing");//实例化铲子(动和静两种)
    CSprite         *chanziCao=new CSprite("chanzicao");                                        //实例化铲子槽

    CSprite         *menu       =   new CSprite("menu");                                        //菜单
    CSprite         *theMenu    =   new CSprite("themenu");                                     //菜单按键
    CSprite         *chooseYes  =   new CSprite("chooseyes");                                   //确认键
    CSprite         *reGame         =   new CSprite("regame");                                  //重新开始键
    CSprite         *oneMoreGame    =   new CSprite("onemoregame");                             //再来亿次

    CSprite         *zombiesWon     =   new CSprite("zombieswon");
    CSprite         *plantsWon      =   new CSprite("plantswon");

    CSprite         *lbwnb          =   new CSprite("overload");
    CSprite         *aoerjia        =   new CSprite("gogogo");

    CSprite         *zombies[5]={
                        new CSprite("zombie_go"),
                        new CSprite("zombie_eat"),
                        new CSprite("zombie_cap"),
                        new CSprite("zombie_iron"),
                        new CSprite("zombie_ash")
    };
    CSprite         *plants[5]={
                        new CSprite("sunflower"),
                        new CSprite("wall1"),
                        new CSprite("peashooter"),
                        new CSprite("cherry"),
                        new CSprite("repeater")
    };
    CardCSprite     *card_plants[5]={
                        new CardCSprite("card_sunflower",0),
                        new CardCSprite("card_wall",1),
                        new CardCSprite("card_peashooter",2),
                        new CardCSprite("card_cherry",3),
                        new CardCSprite("card_repeater",4)
    };
    CSound          *bgm[8]={
                        new CSound("classic(pool).wav",true,0.9),
                        new CSound("classic(singledog).wav",true,0.9),
                        new CSound("classic(zombieking).wav",true,0.9),
                        new CSound("doudizhu.wav",true,0.9),
                        new CSound("clock.wav",true,1),
                        new CSound("civilization.wav",true,0.9),
                        new CSound("jojo.wav",true,0.7),
                        new CSound("spian.wav",true,0.9)
    };
    CSound          *dedede[2]={
                        new CSound("boomdedede.wav",false,0.6),
                        new CSound("dedede.wav",false,0.6)
    };
    CSprite         *music_AoErJia[9]={
                        new CSprite("aoerjiam0"),
                        new CSprite("aoerjiam1"),
                        new CSprite("aoerjiam2"),
                        new CSprite("aoerjiam3"),
                        new CSprite("aoerjiam4"),
                        new CSprite("aoerjiam5"),
                        new CSprite("aoerjiam6"),
                        new CSprite("aoerjiam7"),
                        new CSprite("aoerjiam8")
    };
    CTextSprite     *sunLightText=new CTextSprite("sunlighttext");
    CTextSprite     *cardText[5]={
                        new CTextSprite("cardtext1"),
                        new CTextSprite("cardtext2"),
                        new CTextSprite("cardtext3"),
                        new CTextSprite("cardtext4"),
                        new CTextSprite("cardtext5")
    };
    CSprite         *focusOne[4]={
                        new CSprite("fonechoice"),
                        new CSprite("foneeasy"),
                        new CSprite("fonenormal"),
                        new CSprite("foneold")
    };


    LifeCSprite     *cell_plant[9][5];                                                          // 植物格
    LifeCSprite     *zombie_body[ZOMBIE_SUM][4];
    PeaCSprite      *pea[275];
    CSprite         *hover;     //悬浮单位
    CSprite         *shadow;    //影子单位
    LifeCSprite     *sunLight[10];
    CSprite         *sunLightClick[22];


	CGameMain();            //构造函数
	~CGameMain();           //析构函数

	// Get方法
	int				GetGameState()									{ return m_iGameState; }
	int             getGameFocus()                                  { return gameFocus; }
	int             getGameLevel()                                  { return gameLevel; }
	int             getStartTime()                                  { return startTime; }
	int             getMouseCarry()                                 { return mouseCarry; }
	int             getFiled()                                      { return filed; }
	int             getCardChoose()                                 { return cardChoose; }
	int             getCellHasPlant(int row,int col)                { return cellHas_plant[row][col] ;} //需要有X>=LINE_LEFT && X<=LINE_RIGHT&&Y>=LINE_TOP && Y<=LINE_BOTTOM条件作保障
    int             getRowZombieNum(int index )                     { return rowZombieNum[index]; }
    int             getZombieRemain()                               { return zombieRemain; }
    int             getSunLightNum()                                { return sunLightNum; }
    int             getSunLightCreatTimeSpan()                      { return sunLightCreatTimeSpan; }
    int             getLastSunLightCreatTime()                      { return lastSunLightCreatTime; }
    int             getHasSunLight(int index)                       { return hasSunLight[index]; }
    int             getMySunLight()                                 { return mySunLight; }
    int             getZombieState()                                { return zombieState; }
    int             getZombieList(int index)                        { return zombieList[index]; }
    int             getZombieCreatWaitTime()                        { return zombieCreatWaitTime; }
    int             getZombieCreatNum()                             { return zombieCreatNum; }
    int             getZombieLastCreateTime()                       { return zombieLastCreateTime; }
    int             getPeaTurn()                                    { return peaTurn; }
    int             getHasZombie(int index )                        { return hasZombie[index]; }
    int             getMusicState()                                 { return musicState; }
    int             getSunLightClickIndex()                         { return sunLightClickIndex; }
    int             getLbwUsed()                                    { return lbwUsed; }

	// Set方法
	void			SetGameState( const int iState )				{ m_iGameState	            =	iState; }
	void            setGameFocus( const int value )                 { gameFocus                 =   value; }
	void            setGameLevel( int value )                       { gameLevel                 =   value; }
	void            setStartTime( int value )                       { startTime                 =   value; }
	void            setMouseCarry( const int mouseState )           { mouseCarry                =   mouseState; }
	void            setFiled(int x)                                 { filed                     =   x; }
	void            setCardChoose( int index )                      { cardChoose                =   index; }
	void            setCellHasPlant(int x,int y,int hasState)       { cellHas_plant[x][y]       =   hasState; } //需要有X>=LINE_LEFT && X<=LINE_RIGHT&&Y>=LINE_TOP && Y<=LINE_BOTTOM条件作保障
	void            setCellHasPlantNone()                           { memset(cellHas_plant,-1,sizeof(cellHas_plant)); }
	void            setRowZombieNum(int index,int value)            { rowZombieNum[index]       =   value; }
	void            setRowZombieNumNone()                           { memset(rowZombieNum,0,sizeof(rowZombieNum)); }
	void            setZombieRemain(int value)                      { zombieRemain              =   value; }
	void            setSunLightNum(int value)                       { sunLightNum               =   value; }
	void            setHasSunLight(int index,int value)             { hasSunLight[index]        =   value; }
	void            setHasSunLightNone()                            { memset(hasSunLight,0,sizeof(hasSunLight)); }
	void            setSunLightCreatTimeSpan(int value)             { sunLightCreatTimeSpan     =   value; }
	void            setLastSunLightCreatTime(int value)             { lastSunLightCreatTime     =   value; }
	void            setMySunLight(int value)                        { mySunLight                =   value; sunLightText->SetTextValue(value); }
	void            setZombieState(int value);
	void            setZombieList(int v1,int v2,int v3,int v4,int v5);
	void            setZombiePower(int value);
	void            setZombieCreatWaitTime(int time )               { zombieCreatWaitTime       =   time; }
	void            setZombieCreatNum(int num)                      { zombieCreatNum            =   num; }
	void            setZombieLastCreateTime(int time)               { zombieLastCreateTime      =   time; }
	void            setPeaTurn(int value)                           { peaTurn                   =   value; }
	void            setHasZombie(int index,int value)               { hasZombie[index]          =   value; }
	void            setHasZombieNone()                              { memset(hasZombie,-1,sizeof(hasZombie)); }
	void            setMusicState(int value)                        { musicState                =   value; }
	void            setSunLightClickIndex(int value )               { sunLightClickIndex        =   value; }
	void            setCardLastCreateZero();
	void            setLbwUsed(int value)                           { lbwUsed                   =   value; }

	// 游戏主循环等
	void			GameMainLoop( float	fDeltaTime );
	void			GameInit();
	void			GameRun( float fDeltaTime );
	void			GameEnd();

//=======================================================================================================
    //自定方法

	//创建名为"_hover"的精灵作为悬浮单位,用于随鼠标移动(在CSystem::OnMouseMove中使悬浮单位随着鼠标移动)
	void            creatHover(CSprite *sprite);

	//创建名为"_shadow"的精灵作为影子单位,使玩家直到单位的放置位置(在CSystem::OnMouseMove中控制影子单位移动移动)
	void            creatShadow(CSprite *sprite);

	//检测鼠标是否在草地范围X>=LINE_LEFT && X<=LINE_RIGHT&&Y>=LINE_TOP && Y<=LINE_BOTTOM之内
	//是返回1，不是返回0
	int             isMouseInGrass(const float X,const float Y);

	//返回影子应该存在的XY坐标，用于影子单位移动,需要有X>=LINE_LEFT && X<=LINE_RIGHT&&Y>=LINE_TOP && Y<=LINE_BOTTOM条件作保障
    float           getShadowX(const float X);
    float           getShadowY(const float Y);

    //返回影子应该存在的cellHasPlant数组位置,需要有X>=LINE_LEFT && X<=LINE_RIGHT&&Y>=LINE_TOP && Y<=LINE_BOTTOM条件作保障
    int             getShadowCellX(const float X);
    int             getShadowCellY(const float Y);

    //删除与重置上一局的精灵
    void            initialSprite();

    //设置卡片cd  1：简单 普通模式的cd 2：老年人模式的cd
    void            setTotalCardCD( int state);


};

/////////////////////////////////////////////////////////////////////////////////
//
extern CGameMain	g_GameMain;

#endif // _LESSON_X_H_
