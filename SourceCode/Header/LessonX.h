/////////////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////
#ifndef _LESSON_X_H_
#define _LESSON_X_H_
//

#define LINE_LEFT -34       //��ƺ����
#define LINE_TOP -26        //��ƺ����
#define LINE_RIGHT 48       //��ƺ����
#define LINE_BOTTOM 30      //��ƺ����
#define CELL_WIDTH 9        //��ƺ���
#define CELL_HEIGHT 11.3    //��ƺ���
#define OFFSET_WIDTH 4      //ƫ�ƣ�������ൽ���ĵľ���
#define OFFSET_HEIGHT 8     //ƫ�ƣ������Ϸ������ĵľ���

#define ZOMBIE_SUM 75       //��ʬ���� ��Ҫ����99

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
    //���캯��
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
            hp          =1600;//���ǵڶ���̬�ļ��ǽ
            break;
        case 102:
            hp          =800;//���ǵ�����̬�ļ��ǽ
            break;
        }
    };
    //~LifeCSprite()
    //{

    //}

    //get����
    int             getHp()                         { return hp; }
    int             getCellX()                      { return cellX; }
    int             getCellY()                      { return cellY; }
    int             getCardIndex()                  { return cardIndex; }
    int             getCreateTime()                 { return creatTime; }
    int             getLastShoot()                  { return lastShoot; }
    int             getHasCap()                     { return hasCap; }
    int             getIsOverload()                 { return isOverload; }
    int             getOverloadTime()               { return overloadTime; }

    //set����
    void            setHp( int toState )            { hp            =   toState; }
    void            setLastShoot(int value)         { lastShoot     =   value; }
    void            setHasCap(int value)            { hasCap        =   value; }
    void            setIsOverload(int value)        { isOverload    =   value; }
    void            setOverloadTime(int time)       { overloadTime  =   time; }
};



/////////////////////////////////////////////////////////////////////////////////
//
// ��Ϸ�ܹ��ࡣ��������Ϸ��ѭ������Ϸ��ʼ���������ȹ���
class	CGameMain
{
private:
	int				m_iGameState;				// ��Ϸ״̬��0���������ߵȴ���ʼ��1����ʼ����2����Ϸ������
	int             gameFocus;                  // ��Ϸ����  1����ʼ  2����Ϸ  3���˵� 4����
	int             gameLevel;                  // ��Ϸ�Ѷ� 0���� 1����ͨ 2��������ģʽ
	int             startTime;
    int             mouseCarry;                 // ���״̬��0��δ�������壻1������ֲ�� 2:���� 3:��ʬ 4:LBW�õõá�Х
    int             filed;                      //1��ʧ�� 0��δʧ��
    int             cellHas_plant[9][5];       // ÿ���ݸ��ϵ�ֲ�����״̬ ������Ϊ�������ڵ���0������ڣ�����ֲ����plants�����е�˳����0��ͷ
    int             cardChoose;                 //���ѡ�����ĸ���Ƭ��δѡ��Ϊ����
    int             rowZombieNum[5];
    int             zombieRemain;                //�Ż������Ľ�ʬ����
    int             zombieState;                //����ʬ�ķ�ʽ  1��ǰ�� 2������ ,3:���� 4�����һ�� 5���������
    int             zombiePower;                //��ʬ������ǿ��,���ø����Ե�set��������zombieList����
    int             zombieList[5];              //��ʬ�б��������ˢ��ʬ��1����ͨ��ʬ2��·�Ͻ�ʬ3����Ͱ��ʬ����ͬ����Խ�࣬�����ͽ�ʬˢ�¸���Խ��
    int             zombieCreatWaitTime;        //������ʬˢ�µ�ʱ����
    int             zombieCreatNum;             //һ��ˢ�½�ʬ������
    int             zombieLastCreateTime;       //�ϴ�ˢ��ʬ��ʱ��
    int             hasZombie[ZOMBIE_SUM];      //���zombie_body��Ӧ�±��Ƿ�ָ��һ����ʬ����  �ǣ�1 ���ǣ�0
    int             sunLightNum;
    int             hasSunLight[10];
    int             sunLightClickIndex;
    int             lastSunLightCreatTime;
    int             mySunLight;
    int             sunLightCreatTimeSpan;      //����ˢ��ʱ����
    int             peaTurn;                    //�㶹���
    int             musicState;                 //���� -1������������ �������������� �����SCound*����bgm
    int             lbwUsed;

public:
    CSprite         *chanziDong=new CSprite("chanzidong"),*chanziJing=new CSprite("chanzijing");//ʵ��������(���;�����)
    CSprite         *chanziCao=new CSprite("chanzicao");                                        //ʵ�������Ӳ�

    CSprite         *menu       =   new CSprite("menu");                                        //�˵�
    CSprite         *theMenu    =   new CSprite("themenu");                                     //�˵�����
    CSprite         *chooseYes  =   new CSprite("chooseyes");                                   //ȷ�ϼ�
    CSprite         *reGame         =   new CSprite("regame");                                  //���¿�ʼ��
    CSprite         *oneMoreGame    =   new CSprite("onemoregame");                             //�����ڴ�

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


    LifeCSprite     *cell_plant[9][5];                                                          // ֲ���
    LifeCSprite     *zombie_body[ZOMBIE_SUM][4];
    PeaCSprite      *pea[275];
    CSprite         *hover;     //������λ
    CSprite         *shadow;    //Ӱ�ӵ�λ
    LifeCSprite     *sunLight[10];
    CSprite         *sunLightClick[22];


	CGameMain();            //���캯��
	~CGameMain();           //��������

	// Get����
	int				GetGameState()									{ return m_iGameState; }
	int             getGameFocus()                                  { return gameFocus; }
	int             getGameLevel()                                  { return gameLevel; }
	int             getStartTime()                                  { return startTime; }
	int             getMouseCarry()                                 { return mouseCarry; }
	int             getFiled()                                      { return filed; }
	int             getCardChoose()                                 { return cardChoose; }
	int             getCellHasPlant(int row,int col)                { return cellHas_plant[row][col] ;} //��Ҫ��X>=LINE_LEFT && X<=LINE_RIGHT&&Y>=LINE_TOP && Y<=LINE_BOTTOM����������
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

	// Set����
	void			SetGameState( const int iState )				{ m_iGameState	            =	iState; }
	void            setGameFocus( const int value )                 { gameFocus                 =   value; }
	void            setGameLevel( int value )                       { gameLevel                 =   value; }
	void            setStartTime( int value )                       { startTime                 =   value; }
	void            setMouseCarry( const int mouseState )           { mouseCarry                =   mouseState; }
	void            setFiled(int x)                                 { filed                     =   x; }
	void            setCardChoose( int index )                      { cardChoose                =   index; }
	void            setCellHasPlant(int x,int y,int hasState)       { cellHas_plant[x][y]       =   hasState; } //��Ҫ��X>=LINE_LEFT && X<=LINE_RIGHT&&Y>=LINE_TOP && Y<=LINE_BOTTOM����������
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

	// ��Ϸ��ѭ����
	void			GameMainLoop( float	fDeltaTime );
	void			GameInit();
	void			GameRun( float fDeltaTime );
	void			GameEnd();

//=======================================================================================================
    //�Զ�����

	//������Ϊ"_hover"�ľ�����Ϊ������λ,����������ƶ�(��CSystem::OnMouseMove��ʹ������λ��������ƶ�)
	void            creatHover(CSprite *sprite);

	//������Ϊ"_shadow"�ľ�����ΪӰ�ӵ�λ,ʹ���ֱ����λ�ķ���λ��(��CSystem::OnMouseMove�п���Ӱ�ӵ�λ�ƶ��ƶ�)
	void            creatShadow(CSprite *sprite);

	//�������Ƿ��ڲݵط�ΧX>=LINE_LEFT && X<=LINE_RIGHT&&Y>=LINE_TOP && Y<=LINE_BOTTOM֮��
	//�Ƿ���1�����Ƿ���0
	int             isMouseInGrass(const float X,const float Y);

	//����Ӱ��Ӧ�ô��ڵ�XY���꣬����Ӱ�ӵ�λ�ƶ�,��Ҫ��X>=LINE_LEFT && X<=LINE_RIGHT&&Y>=LINE_TOP && Y<=LINE_BOTTOM����������
    float           getShadowX(const float X);
    float           getShadowY(const float Y);

    //����Ӱ��Ӧ�ô��ڵ�cellHasPlant����λ��,��Ҫ��X>=LINE_LEFT && X<=LINE_RIGHT&&Y>=LINE_TOP && Y<=LINE_BOTTOM����������
    int             getShadowCellX(const float X);
    int             getShadowCellY(const float Y);

    //ɾ����������һ�ֵľ���
    void            initialSprite();

    //���ÿ�Ƭcd  1���� ��ͨģʽ��cd 2��������ģʽ��cd
    void            setTotalCardCD( int state);


};

/////////////////////////////////////////////////////////////////////////////////
//
extern CGameMain	g_GameMain;

#endif // _LESSON_X_H_
