/////////////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////
#define rdm(x) rand()%(x)

#include <Stdio.h>
#include <cstdlib>
#include <ctime>
#include "CommonClass.h"
#include "LessonX.h"
//多个文件
//整块写注释
////////////////////////////////////////////////////////////////////////////////
//
//
CGameMain		g_GameMain;

//==============================================================================
//
// 大体的程序流程为：GameMainLoop函数为主循环函数，在引擎每帧刷新屏幕图像之后，都会被调用一次。

//==============================================================================
//
// 构造函数
CGameMain::CGameMain()
{
	m_iGameState			=	0;
    setGameFocus(1);
    setGameLevel(0);
    setMusicState(1);

}
//==============================================================================
//
// 析构函数
CGameMain::~CGameMain()
{
}

//==============================================================================
//
// 游戏主循环，此函数将被不停的调用，引擎每刷新一次屏幕，此函数即被调用一次
// 用以处理游戏的开始、进行中、结束等各种状态.
// 函数参数fDeltaTime : 上次调用本函数到此次调用本函数的时间间隔，单位：秒
void CGameMain::GameMainLoop( float	fDeltaTime )
{
	switch( GetGameState() )
	{
		// 初始化游戏，清空上一局相关数据
	case 1:
		{
			GameInit();
			SetGameState(2); // 初始化之后，将游戏状态设置为进行中
		}
		break;

		// 游戏进行中，处理各种游戏逻辑
	case 2:
		{
			// TODO 修改此处游戏循环条件，完成正确游戏逻辑
			if( getGameFocus()==2 || getGameFocus()==3 )
			{
				GameRun( fDeltaTime );
			}
			else // 游戏结束。调用游戏结算函数，并把游戏状态修改为结束状态
			{
				SetGameState(0);
				GameEnd();
			}
		}
		break;

		// 游戏结束/等待按空格键开始
	case 0:
	default:
		break;
	};
}
//=============================================================================
//
// 每局开始前进行初始化，清空上一局相关数据
void CGameMain::GameInit()
{
    bgm[getMusicState()]->PlaySoundA();
    setGameFocus(2);
	setMouseCarry(0);
	setCellHasPlantNone();
	setCardChoose(-1);
	setRowZombieNumNone();
	setZombieRemain(ZOMBIE_SUM);
	setSunLightNum(0);
	setHasSunLightNone();
	setZombieState(1);
    setZombieLastCreateTime(getStartTime()+15);
    setPeaTurn(0);
    setHasZombieNone();
    setLastSunLightCreatTime(getStartTime());
    setSunLightCreatTimeSpan(9);
    setSunLightClickIndex(0);
    setFiled(0);
    setCardLastCreateZero();
    setLbwUsed(0);
    if(getGameLevel()==2){
        setMySunLight(9000);
        setTotalCardCD(2);
    }else if(getGameLevel()==1 || getGameLevel()==0){
        setMySunLight(50);
        setTotalCardCD(1);
    }
}

//=============================================================================
//
// 每局游戏进行中
void CGameMain::GameRun( float fDeltaTime )
{
    int hasZombie=0;
    if(getZombieRemain()<=0){
        for(int i=0; i<ZOMBIE_SUM; i++){
            if(getHasZombie(i)>0){
                hasZombie=1;
                break;
            }
        }
        if(hasZombie==0){
            setGameFocus(4);
            return ;
        }
    }
    int nowTime=clock()/CLOCKS_PER_SEC;
if(getGameFocus()==2 || getGameFocus()==3){
    //改变刷僵尸机制
    if(getZombieState()!=5){  //如果不是死亡冲锋模式
        if(getZombieState()==1 && getZombieRemain()<=ZOMBIE_SUM-5){
            setZombieState(2);
        }else if(getZombieState()==2 && getZombieRemain()<=ZOMBIE_SUM-15){
            setZombieState(3);
        }else if(getZombieState()==3 && getZombieRemain()<=(int)(ZOMBIE_SUM/2)){
            setZombieState(4);
        }
    }
    //刷新卡片冷却
    for(int i=0; i<5; i++){
        int nowCD =card_plants[i]->getCD()-(nowTime-card_plants[i]->getLastCreate());
        if( nowCD > 0){
            cardText[i]->SetTextValue(nowCD);
        }else if( nowCD == 0){
            cardText[i]->SetTextString("临");
            card_plants[i]->SetSpriteColorRed(255);  //恢复卡片颜色
        }else{
            cardText[i]->SetTextChar(' ');
        }
    }
    //刷僵尸
    if(nowTime-getZombieLastCreateTime()>getZombieCreatWaitTime()){
        setZombieLastCreateTime(nowTime);
        for(int i=0; i<getZombieCreatNum() && getZombieRemain()>0; i++){
            int index=rdm(5),pos=rdm(5);//index僵尸种类，pos僵尸所在的行
            char name[20]="zombie  ",eat[5]="eat",cap[5]="cap";
            char eatname[20],capname[20];
            name[6]=(char)(((getZombieRemain()-1)/10)%10)+48;
            name[7]=(char)((getZombieRemain()-1)%10)+48;
            strcpy(eatname,name);
            strcpy(capname,name);
            strcat(eatname,eat);
            strcat(capname,cap);
            switch(zombieList[index])
            {
            case 1://普通僵尸
                zombie_body[getZombieRemain()-1][0]=new LifeCSprite(name,zombies[0]->GetName(),-5,pos,5);
                zombie_body[getZombieRemain()-1][1]=new LifeCSprite(strcat(eatname,zombies[1]->GetName()),zombies[1]->GetName(),-5,pos,-5);
                zombie_body[getZombieRemain()-1][1]->SpriteMountToSprite(zombie_body[getZombieRemain()-1][0]->GetName(),0,0);
                zombie_body[getZombieRemain()-1][1]->SetSpriteColorAlpha(0);
                zombie_body[getZombieRemain()-1][0]->SetSpritePosition(52+0.2*i,LINE_TOP+pos*CELL_HEIGHT+OFFSET_HEIGHT);
                zombie_body[getZombieRemain()-1][0]->SetSpriteLinearVelocityX(-4);
                setHasZombie(getZombieRemain()-1,1);
                setZombieRemain(getZombieRemain()-1);
                setRowZombieNum(pos,getRowZombieNum(pos)+1);
            break;
            case 2://路障僵尸
                zombie_body[getZombieRemain()-1][0]=new LifeCSprite(name,zombies[0]->GetName(),-5,pos,6);
                zombie_body[getZombieRemain()-1][1]=new LifeCSprite(strcat(eatname,zombies[1]->GetName()),zombies[1]->GetName(),-15,pos,-5);
                zombie_body[getZombieRemain()-1][1]->SpriteMountToSprite(zombie_body[getZombieRemain()-1][0]->GetName(),0,0);
                zombie_body[getZombieRemain()-1][1]->SetSpriteColorAlpha(0);
                zombie_body[getZombieRemain()-1][2]=new LifeCSprite(strcat(capname,zombies[2]->GetName()),zombies[2]->GetName(),-5,pos,-5);
                zombie_body[getZombieRemain()-1][2]->SpriteMountToSprite(zombie_body[getZombieRemain()-1][0]->GetName(),-0.1,-0.9);
                zombie_body[getZombieRemain()-1][0]->SetSpritePosition(52+0.2*i,LINE_TOP+pos*CELL_HEIGHT+OFFSET_HEIGHT);
                zombie_body[getZombieRemain()-1][0]->SetSpriteLinearVelocityX(-4);
                setHasZombie(getZombieRemain()-1,1);
                setZombieRemain(getZombieRemain()-1);
                setRowZombieNum(pos,getRowZombieNum(pos)+1);
            break;
            case 3://铁桶僵尸
                zombie_body[getZombieRemain()-1][0]=new LifeCSprite(name,zombies[0]->GetName(),-5,pos,7);
                zombie_body[getZombieRemain()-1][1]=new LifeCSprite(strcat(eatname,zombies[1]->GetName()),zombies[1]->GetName(),-5,pos,-5);
                zombie_body[getZombieRemain()-1][1]->SpriteMountToSprite(zombie_body[getZombieRemain()-1][0]->GetName(),0,0);
                zombie_body[getZombieRemain()-1][1]->SetSpriteColorAlpha(0);
                zombie_body[getZombieRemain()-1][2]=new LifeCSprite(strcat(capname,zombies[3]->GetName()),zombies[3]->GetName(),-5,pos,-5);
                zombie_body[getZombieRemain()-1][2]->SpriteMountToSprite(zombie_body[getZombieRemain()-1][0]->GetName(),-0.1,-0.8);
                zombie_body[getZombieRemain()-1][0]->SetSpritePosition(52+0.2*i,LINE_TOP+pos*CELL_HEIGHT+OFFSET_HEIGHT);
                zombie_body[getZombieRemain()-1][0]->SetSpriteLinearVelocityX(-4);
                setHasZombie(getZombieRemain()-1,1);
                setZombieRemain(getZombieRemain()-1);
                setRowZombieNum(pos,getRowZombieNum(pos)+1);
            break;
            }
        }
    }

    //植物操作
    for(int i=0; i<5; i++){
        for(int j=0; j<9; j++){
            if(getCellHasPlant(j,i)>=0){
                if(cell_plant[j][i]->getCardIndex()==4 || cell_plant[j][i]->getCardIndex()==2 ){    //如果这个位置的植物是射手
                    if(nowTime-cell_plant[j][i]->getLastShoot()>=1){
                        int bulletNum=0;
                        switch(cell_plant[j][i]->getCardIndex())
                        {
                        case 2:     bulletNum=1;    break;      //单次发射豌豆是数目
                        case 4:     bulletNum=2;    break;
                        }
                        if(getRowZombieNum(i)>0){               //如果这一行有僵尸，就发射豌豆
                            for(int k=0; k<bulletNum; k++){
                                char name[20]="bullet   ";
                                name[6]=(char)(getPeaTurn()/100+48);
                                name[7]=(char)((getPeaTurn()/10)%10)+48;
                                name[8]=(char)(getPeaTurn()%10)+48;
                                pea[getPeaTurn()]=new PeaCSprite(name,"pea",getPeaTurn());
                                pea[getPeaTurn()]->SetSpriteLifeTime(2);
                                pea[getPeaTurn()]->SetSpriteLinearVelocity(50,0);
                                pea[getPeaTurn()]->SetSpritePosition(cell_plant[j][i]->GetSpritePositionX()+2*k,cell_plant[j][i]->GetSpritePositionY()-2);
                                setPeaTurn((getPeaTurn()+1)%270);   //当豌豆精灵指针数组下标超出给定数值，重置下标
                            }
                        }
                        if(cell_plant[j][i]->getIsOverload()>0){    //检测植物是否过载（使用了LBW得得得得得得得啸）
                            if(nowTime-cell_plant[j][i]->getOverloadTime()>=7){
                                cell_plant[j][i]->DeleteSprite();
                                setCellHasPlant(j,i,-1);
                            }
                        }else{
                            cell_plant[j][i]->setLastShoot(nowTime);    //如果没有过载，把上次发射豌豆的时间设为“现在”
                        }
                    }
                }else if(cell_plant[j][i]->getCardIndex()==0){          //如果这一格是向日葵，按时产生阳光
                    if(nowTime-cell_plant[j][i]->getLastShoot()>=getSunLightCreatTimeSpan() && getSunLightNum()<10){
                        for(int k=0; k<10; k++){
                            if(getHasSunLight(k)==0){
                                char name[15]="sunlight  ";
                                name[8]=(char)(k+48);
                                sunLight[k]=new LifeCSprite(name,"sunlight",-5,-5,-5);
                                sunLight[k]->SetSpritePosition(cell_plant[j][i]->GetSpritePositionX(),cell_plant[j][i]->GetSpritePositionY());
                                setHasSunLight(k,1);
                                cell_plant[j][i]->setLastShoot(nowTime);
                                break;
                            }
                        }
                    }
                }else if(cell_plant[j][i]->getCardIndex()==3){              //如果这一格是樱桃炸弹
                    if(nowTime-cell_plant[j][i]->getCreateTime()==1){
                        dedede[0]->PlaySoundA();                //播放爆炸音效
                    }
                    if(nowTime-cell_plant[j][i]->getCreateTime()==2){
                        int posX=cell_plant[j][i]->GetSpritePositionX(),posY=cell_plant[j][i]->GetSpritePositionY();
                        cell_plant[j][i]->DeleteSprite();
                        setCellHasPlant(j,i,-1);
                        for(int k=((getZombieRemain()-6)>0 ? getZombieRemain()-6 : 0); k<ZOMBIE_SUM; k++){
                            if(getHasZombie(k)>0){
                                if(zombie_body[k][0]->IsPointInSprite(posX,posY)||zombie_body[k][0]->IsPointInSprite(posX+5,posY)||
                                   zombie_body[k][0]->IsPointInSprite(posX+9,posY)||zombie_body[k][0]->IsPointInSprite(posX-5,posY)||
                                   zombie_body[k][0]->IsPointInSprite(posX-9,posY)||zombie_body[k][0]->IsPointInSprite(posX,posY+6)||
                                   zombie_body[k][0]->IsPointInSprite(posX,posY+11)||zombie_body[k][0]->IsPointInSprite(posX,posY-6)||
                                   zombie_body[k][0]->IsPointInSprite(posX,posY-11)||zombie_body[k][0]->IsPointInSprite(posX+5,posY+11)||
                                   zombie_body[k][0]->IsPointInSprite(posX-5,posY-11)||zombie_body[k][0]->IsPointInSprite(posX+5,posY-11)||
                                   zombie_body[k][0]->IsPointInSprite(posX-5,posY+11)||zombie_body[k][0]->IsPointInSprite(posX-11,posY)||
                                   zombie_body[k][0]->IsPointInSprite(posX+13,posY)||zombie_body[k][0]->IsPointInSprite(posX-16,posY)||
                                   zombie_body[k][0]->IsPointInSprite(posX+13,posY+11)||zombie_body[k][0]->IsPointInSprite(posX+13,posY-11)||
                                   zombie_body[k][0]->IsPointInSprite(posX-13,posY+11)||zombie_body[k][0]->IsPointInSprite(posX-13,posY-11)||
                                   zombie_body[k][0]->IsPointInSprite(posX+9,posY)||zombie_body[k][0]->IsPointInSprite(posX-9,posY)||
                                   zombie_body[k][0]->IsPointInSprite(posX+9,posY+11)||zombie_body[k][0]->IsPointInSprite(posX+9,posY-11)||
                                   zombie_body[k][0]->IsPointInSprite(posX-9,posY+11)||zombie_body[k][0]->IsPointInSprite(posX-9,posY-11)){
                                       int zombiePosX=zombie_body[k][0]->GetSpritePositionX(),zombiePosY=zombie_body[k][0]->GetSpritePositionY();//在这些位置的僵尸死亡
                                       int row=zombie_body[k][0]->getCellY();
                                       zombie_body[k][0]->DeleteSprite();
                                       setHasZombie(k,0);
                                       setRowZombieNum(row,getRowZombieNum(row)-1);
                                       char name[15]="ash   ";
                                       name[3]=(char)(k/10)+48;
                                       name[4]=(char)(k%10)+48;
                                       zombie_body[k][3]=new LifeCSprite(name,"zombie_ash",-5,-5,-5);       //僵尸灰烬精灵
                                       zombie_body[k][3]->SetSpritePosition(zombiePosX,zombiePosY);
                                       zombie_body[k][3]->SetSpriteLifeTime(1.8);
                                   }

                            }
                        }
                    }
                }
            }
        }
    }

    //天上刷阳光 and 删除过长时间不捡的阳光，有小bug
    if(nowTime-getLastSunLightCreatTime()>=getSunLightCreatTimeSpan() && getSunLightNum()<10){
        int hascreate=-1;
        for(int i=0; i<10; i++){
            if(getHasSunLight(i)==0){
                if(hascreate<0){
                    int posX=rdm(9)*CELL_WIDTH+LINE_LEFT,posY=-35;
                    char name[15]="sunlight  ";
                    name[8]=(char)(i+48);
                    sunLight[i]=new LifeCSprite(name,"sunlight",-5,-5,-5);
                    sunLight[i]->SetSpritePosition(posX,posY);
                    sunLight[i]->SetSpriteLinearVelocity(0,4);
                    setHasSunLight(i,1);
                    setLastSunLightCreatTime(nowTime);
                    hascreate=1;
                }
            }else{
                if(nowTime-sunLight[i]->getCreateTime()>=15){
                    sunLight[i]->DeleteSprite();
                    setHasSunLight(i,0);
                }
            }
        }
    }

}
}
//=============================================================================
//
// 本局游戏结束
void CGameMain::GameEnd()
{
    //主要是清除工作和界面位置的移动
    for(int i=0; i<ZOMBIE_SUM; i++){
        if(getHasZombie(i)>0){
            zombie_body[i][0]->SetSpriteLinearVelocity(0,0);
        }
    }
    if(getFiled()==0){
        plantsWon->SetSpritePosition(0,0);
    }else{
        zombiesWon->SetSpritePosition(0,0);
    }
    oneMoreGame->SetSpritePosition(-20,32);
    if(getMusicState()>=0){
        bgm[getMusicState()]->StopSound();
    }
    menu->SetSpritePosition(135,135);
    reGame->SetSpritePosition(100,-100);
    chanziJing->SetSpritePosition(-25,-34);
    hover->DeleteSprite();
    shadow->DeleteSprite();
}

//______________________________________________________________________________________________
//----------------------------------------------------------------------------------------------
//**********************************************************************************************
//                      <-- 自定方法 -->
//创建名为"_hover"的精灵作为悬浮单位,用于随鼠标移动(在CSystem::OnMouseMove中使悬浮单位随着鼠标移动)
void CGameMain::creatHover(CSprite *sprite)
{
    hover=new CSprite("_hover",sprite->GetName());
}
//-----------------------------------------------------------------------------------------------------------
//创建名为"_shadow"的精灵作为影子单位,使玩家直到单位的放置位置(在CSystem::OnMouseMove中控制影子单位移动移动)
void CGameMain::creatShadow(CSprite *sprite)
{
    shadow=new CSprite("_shadow",sprite->GetName());
    shadow->SetSpriteColorAlpha(100);
}
//-----------------------------------------------------------------------------------------------------
//检测鼠标是否在草地范围X>=LINE_LEFT && X<=LINE_RIGHT&&Y>=LINE_TOP && Y<=LINE_BOTTOM之内
//是返回1，不是返回0
int CGameMain::isMouseInGrass(const float X,const float Y)
{
    return (X>=LINE_LEFT && X<=LINE_RIGHT && Y>=LINE_TOP && Y<=LINE_BOTTOM) ? 1 : 0 ;
}
//---------------------------------------------------------------------------------------------------------
//返回影子应该存在的XY坐标，用于影子单位移动,需要有X>=LINE_LEFT && X<=LINE_RIGHT&&Y>=LINE_TOP && Y<=LINE_BOTTOM条件作保障
float CGameMain::getShadowX(const float X)
{
    return ((int)((X-LINE_LEFT)/CELL_WIDTH)*CELL_WIDTH+LINE_LEFT+OFFSET_WIDTH);
}
float CGameMain::getShadowY(const float Y)
{
    return ((int)((Y-LINE_TOP)/CELL_HEIGHT)*CELL_HEIGHT+LINE_TOP+OFFSET_HEIGHT);
}
//----------------------------------------------------------------------------------------------------------
//返回影子应该存在的cellHasPlant数组位置,需要有X>=LINE_LEFT && X<=LINE_RIGHT&&Y>=LINE_TOP && Y<=LINE_BOTTOM条件作保障
int CGameMain::getShadowCellX(const float X)
{
    return (int)((int)(X-LINE_LEFT)/(CELL_WIDTH));
}
int CGameMain::getShadowCellY(const float Y)
{
    return (int)((int)(Y-LINE_TOP)/CELL_HEIGHT);
}
//----------------------------------------------------------------------------------------------------------
//设置出僵尸方式
//出僵尸的方式  1：前期 2：中期 ,3:后期 4：最后一波 5：死亡冲锋
void CGameMain::setZombieState(int value)
{
    zombieState=value;
    switch(gameLevel)
    {
    case 0:
    case 2:
        switch(value)
        {
        case 1:
            setZombiePower(1); setZombieCreatWaitTime(11); setZombieCreatNum(1); break;
        case 2:
            setZombiePower(2); setZombieCreatWaitTime(6); setZombieCreatNum(1); break;
        case 3:
            setZombiePower(3); setZombieCreatWaitTime(4); setZombieCreatNum(2); break;
        case 4:
            setZombiePower(4); setZombieCreatWaitTime(1); setZombieCreatNum(4); break;
        case 5:
            setZombiePower(4); setZombieCreatWaitTime(1); setZombieCreatNum(5); break;
        }
    break;
    case 1:
        switch(value)
        {
        case 1:
            setZombiePower(1); setZombieCreatWaitTime(11); setZombieCreatNum(1); break;
        case 2:
            setZombiePower(3); setZombieCreatWaitTime(7); setZombieCreatNum(2); break;
        case 3:
            setZombiePower(4); setZombieCreatWaitTime(5); setZombieCreatNum(3); break;
        case 4:
            setZombiePower(5); setZombieCreatWaitTime(1); setZombieCreatNum(5); break;
        case 5:
            setZombiePower(5); setZombieCreatWaitTime(1); setZombieCreatNum(6); break;
        }
    break;
    }
}
//----------------------------------------------------------------------------------------------------------
//设置刷僵尸列表1:普通，2：路障，3：铁桶
void CGameMain::setZombieList(int v1,int v2,int v3,int v4,int v5)
{
    zombieList[0]=v1;
    zombieList[1]=v2;
    zombieList[2]=v3;
    zombieList[3]=v4;
    zombieList[4]=v5;

}
//----------------------------------------------------------------------------------------------------------
//设置僵尸整体强度
//参数value=1：全普通，开局难度
//value=2：普通占2/5，路障占3/5
//value=3：普通占1/5，路障占2/5，铁桶占2/5
//value=4：路障占3/5，铁桶占2/5
//value=5：路障占1/5，铁桶占4/5
void CGameMain::setZombiePower(int value)
{
    zombiePower=value;
    switch(zombiePower)
    {
    case 1:
        setZombieList(1,1,1,1,1);
        break;
    case 2:
        setZombieList(2,1,2,1,2);
        break;
    case 3:
        setZombieList(3,2,1,2,3);
        break;
    case 4:
        setZombieList(2,3,2,3,2);
        break;
    case 5:
        setZombieList(3,3,2,3,3);
        break;
    }
}
//----------------------------------------------------------------------------------------------------------
//重置卡片冷却
void CGameMain::setCardLastCreateZero()
{
    for(int i=0; i<5; i++){
        card_plants[i]->setLastCreate(-50);
    }
}
//----------------------------------------------------------------------------------------------------------
//删除与重置上一局的精灵
void CGameMain::initialSprite()
{
    for(int i=0; i<ZOMBIE_SUM; i++){
        if(getHasZombie(i)>0){
            zombie_body[i][0]->DeleteSprite();
            setHasZombie(i,0);
        }
    }
    for(int i=0; i<5; i++){
        for(int j=0; j<9; j++){
            if(getCellHasPlant(j,i)>=0){
                cell_plant[j][i]->DeleteSprite();
                setCellHasPlant(j,i,-1);
            }
        }
    }
    for(int i=0; i<10; i++){
        if(getHasSunLight(i)>0){
            sunLight[i]->DeleteSprite();
            setHasSunLight(i,0);
        }
    }
    for(int i=0; i<5; i++){
        cardText[i]->SetTextChar(' ');
        card_plants[i]->SetSpriteColorRed(255);
    }
    lbwnb->SetSpriteColorRed(255);
    lbwnb->SetSpriteColorAlpha(255);
    aoerjia->SetSpriteColorAlpha(255);
}
//----------------------------------------------------------------------------------------------------------
//设置卡片cd  1：简单 普通模式的cd 2：老年人模式的cd
void CGameMain::setTotalCardCD( int state )
{
    if(state==2){
        for(int i=0; i<5; i++){
            card_plants[i]->setCD(0);
        }
    }else if(state==1){
        for(int i=0; i<5; i++){
            if(i==0 || i==2 || i==4){
                card_plants[i]->setCD(7);
            }else if(i==1){
                card_plants[i]->setCD(27);
            }else if(i==3){
                card_plants[i]->setCD(36);
            }
        }
    }
}
