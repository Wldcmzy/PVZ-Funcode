//-----------------------------------------------------------------------------
// 两个较大bug
//1. 可能由于语句执行先后的时间差，可能出现放置植物时，植物的判定标记位置与植物精灵放置位置不同步
//2. 射手只检测它所在的行有没有僵尸而发射豌豆，而不是检测它前面有没有僵尸
//
//-----------------------------------------------------------------------------
#include "CommonClass.h"
#include "LessonX.h"
#include <stdio.h>
#include <cstdlib>
#include <ctime>

///////////////////////////////////////////////////////////////////////////////////////////
//
// 主函数入口
//
//////////////////////////////////////////////////////////////////////////////////////////
int PASCAL WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR     lpCmdLine,
                   int       nCmdShow)
{
	// 初始化游戏引擎
	if( !CSystem::InitGameEngine( hInstance, lpCmdLine ) )
		return 0;

	// To do : 在此使用API更改窗口标题
	CSystem::SetWindowTitle("PVZ-PoweredBy-SXY");

	// 引擎主循环，处理屏幕图像刷新等工作
	while( CSystem::EngineMainLoop() )
	{
		// 获取两次调用之间的时间差，传递给游戏逻辑处理
		float	fTimeDelta	=	CSystem::GetTimeDelta();

		// 执行游戏主循环
		g_GameMain.GameMainLoop( fTimeDelta );
	};

	// 关闭游戏引擎
	CSystem::ShutdownGameEngine();
	return 0;
}

//==========================================================================
//
// 引擎捕捉鼠标移动消息后，将调用到本函数
void CSystem::OnMouseMove( const float fMouseX, const float fMouseY )
{
	// 可以在此添加游戏需要的响应函数
if(g_GameMain.getGameFocus()==2){
	switch(g_GameMain.getMouseCarry())
	{
    case 1:
        //让植物跟随鼠标移动过
        g_GameMain.hover->SetSpritePosition(fMouseX,fMouseY);                                                           //悬浮单位跟随鼠标移动
        if(g_GameMain.isMouseInGrass(fMouseX,fMouseY)){
            if(g_GameMain.getCellHasPlant(g_GameMain.getShadowCellX(fMouseX),g_GameMain.getShadowCellY(fMouseY)) < 0)     //如果这一格没有植物
                g_GameMain.shadow->SetSpritePosition(g_GameMain.getShadowX(fMouseX),g_GameMain.getShadowY(fMouseY));    //把影子移动到这一格
            else
            g_GameMain.shadow->SetSpritePosition(-100,-100);

        }else
        g_GameMain.shadow->SetSpritePosition(-100,-100);
    break;
    case 2:
        //让铲子跟随鼠标移动过
        g_GameMain.chanziJing->SetSpritePosition(fMouseX,fMouseY);
        if(g_GameMain.isMouseInGrass(fMouseX,fMouseY)){
            if(g_GameMain.getCellHasPlant(g_GameMain.getShadowCellX(fMouseX),g_GameMain.getShadowCellY(fMouseY)) >= 0)        //如果这一格有植物
                g_GameMain.chanziDong->SetSpritePosition(g_GameMain.getShadowX(fMouseX),g_GameMain.getShadowY(fMouseY));
            else
            g_GameMain.chanziDong->SetSpritePosition(-120,-120);
        }else
        g_GameMain.chanziDong->SetSpritePosition(-120,-120);
    break;
    case 4:
        //让过载LBW跟随鼠标移动过
        g_GameMain.hover->SetSpritePosition(fMouseX,fMouseY);
        if(g_GameMain.isMouseInGrass(fMouseX,fMouseY)){
            int cellX=g_GameMain.getShadowCellX(fMouseX),cellY=g_GameMain.getShadowCellY(fMouseY);
            if(g_GameMain.getCellHasPlant(cellX,cellY) >= 0 && (g_GameMain.cell_plant[cellX][cellY]->getCardIndex()==2 || g_GameMain.cell_plant[cellX][cellY]->getCardIndex()==4 ))
                g_GameMain.shadow->SetSpritePosition(g_GameMain.getShadowX(fMouseX),g_GameMain.getShadowY(fMouseY));
            else
                g_GameMain.shadow->SetSpritePosition(-100,-100);
        }else
        g_GameMain.shadow->SetSpritePosition(-100,-100);
    break;
	}
}
}
//==========================================================================
//
// 引擎捕捉鼠标点击消息后，将调用到本函数
void CSystem::OnMouseClick( const int iMouseType, const float fMouseX, const float fMouseY )
{
    int nowTime=clock()/CLOCKS_PER_SEC;
	// 可以在此添加游戏需要的响应函数

if(g_GameMain.getGameFocus()==1){
    //对应选择难度的界面
    for(int i=0; i<3; i++){
        if(g_GameMain.focusOne[i+1]->IsPointInSprite(fMouseX,fMouseY)==true){
            g_GameMain.setGameLevel(i);//设置游戏难度
            for(int j=0; j<3; j++){
                if(j==i){
                    g_GameMain.focusOne[j+1]->SetSpriteColorAlpha(255);
                }else
                g_GameMain.focusOne[j+1]->SetSpriteColorAlpha(50);
            }
            break;
        }
    }
}else if(g_GameMain.getGameFocus()==4){
    //对应游戏结束一次后的界面
    if(g_GameMain.oneMoreGame->IsPointInSprite(fMouseX,fMouseY)==true){
        g_GameMain.initialSprite();
        g_GameMain.zombiesWon->SetSpritePosition(100,-100);
        g_GameMain.plantsWon->SetSpritePosition(100,-100);
        g_GameMain.oneMoreGame->SetSpritePosition(100,-100);
        g_GameMain.focusOne[0]->SetSpritePosition(0,0);
        g_GameMain.SetGameState(0);
        g_GameMain.setGameFocus(1);
    }
}else if(g_GameMain.getGameFocus()==3){
    //对应游戏中点击菜单后的界面
    if(g_GameMain.reGame->IsPointInSprite(fMouseX,fMouseY)==true){
        g_GameMain.initialSprite();
        g_GameMain.menu->SetSpritePosition(135,135);
        g_GameMain.reGame->SetSpritePosition(100,-100);
        g_GameMain.focusOne[0]->SetSpritePosition(0,0);
        g_GameMain.SetGameState(0);
        g_GameMain.setGameFocus(1);
        return;
    }if(g_GameMain.chooseYes->IsPointInSprite(fMouseX,fMouseY)==true){
        g_GameMain.menu->SetSpritePosition(135,135);
        g_GameMain.reGame->SetSpritePosition(100,-100);
        g_GameMain.setGameFocus(2);
    }
    for(int i=0; i<9; i++){
        if(g_GameMain.music_AoErJia[i]->IsPointInSprite(fMouseX,fMouseY)==true){
            if(i==0){
                if(g_GameMain.getMusicState() >= 0){
                    g_GameMain.bgm[g_GameMain.getMusicState()]->StopSound();
                    g_GameMain.setMusicState(i-1);
                }
            }else{
                if(g_GameMain.getMusicState() >= 0){
                    if(g_GameMain.getMusicState() != i-1){
                        g_GameMain.bgm[g_GameMain.getMusicState()]->StopSound();
                        g_GameMain.setMusicState(i-1);
                        g_GameMain.bgm[g_GameMain.getMusicState()]->PlaySoundA();
                    }
                }else{
                    g_GameMain.setMusicState(i-1);
                    g_GameMain.bgm[g_GameMain.getMusicState()]->PlaySoundA();
                }
            }
            for(int j=0; j<9; j++){
                if(j==i){
                    g_GameMain.music_AoErJia[j]->SetSpriteColorAlpha(255);
                }else{
                    g_GameMain.music_AoErJia[j]->SetSpriteColorAlpha(50);
                }
            }
            return ;
        }
    }
}else if(g_GameMain.getGameFocus()==2){
    //对应游戏中
    switch(g_GameMain.getMouseCarry())
    {
    case 0:
        //点击菜单
        if(g_GameMain.theMenu->IsPointInSprite(fMouseX,fMouseY)==true){
            g_GameMain.setGameFocus(3);
            g_GameMain.menu->SetSpritePosition(0,0);
            g_GameMain.reGame->SetSpritePosition(30,33);
            return ;
        }
        //收集阳光
        for(int i=0; i<10; i++){
            if(g_GameMain.getHasSunLight(i)>0){
                if(g_GameMain.sunLight[i]->IsPointInSprite(fMouseX,fMouseY)==true){
                    int posX=g_GameMain.sunLight[i]->GetSpritePositionX(),posY=g_GameMain.sunLight[i]->GetSpritePositionY();
                    int idx=g_GameMain.getSunLightClickIndex();
                    g_GameMain.sunLight[i]->DeleteSprite();
                    g_GameMain.setHasSunLight(i,0);
                    g_GameMain.setMySunLight(g_GameMain.getMySunLight()+25);
                    char name[20]="sunlightclick   ";
                    name[13]=(char)(idx/10+48);
                    name[14]=(char)(idx%10+48);
                    g_GameMain.sunLightClick[idx]=new CSprite(name,"sunlight");
                    g_GameMain.sunLightClick[idx]->SetSpriteColorAlpha(150);
                    g_GameMain.sunLightClick[idx]->SetSpriteLinearVelocity(25,-100);
                    g_GameMain.sunLightClick[idx]->SetSpritePosition(posX,posY);
                    g_GameMain.sunLightClick[idx]->SetSpriteLifeTime(1);
                    g_GameMain.setSunLightClickIndex((g_GameMain.getSunLightClickIndex()+1)%20);
                    return ;
                }
            }
        }
        //点击植物卡片
        for(int i=0; i<5; i++){
            if(g_GameMain.card_plants[i]->IsPointInSprite(fMouseX,fMouseY)==true){
                if(nowTime-g_GameMain.card_plants[i]->getLastCreate() >= g_GameMain.card_plants[i]->getCD()+1 &&
                g_GameMain.getMySunLight() >= g_GameMain.card_plants[i]->getCost()){
                    g_GameMain.card_plants[i]->SetSpriteColorRed(0);                  //卡片颜色变化
                    g_GameMain.creatShadow(g_GameMain.plants[i]);
                    g_GameMain.creatHover(g_GameMain.plants[i]);
                    g_GameMain.setCardChoose(i);
                    g_GameMain.setMouseCarry(1);
                }
                return ;
            }
        }
        //点击LBW得得得得得得得得·啸
        if(g_GameMain.lbwnb->IsPointInSprite(fMouseX,fMouseY)==true){
            if(g_GameMain.getLbwUsed()==0){
                g_GameMain.lbwnb->SetSpriteColorRed(0);
                g_GameMain.creatShadow(g_GameMain.lbwnb);
                g_GameMain.creatHover(g_GameMain.lbwnb);
                g_GameMain.setMouseCarry(4);
            }
            return ;
        }
        //点击“死亡冲锋”  （继续迫害奥尔加团长）
        if(g_GameMain.aoerjia->IsPointInSprite(fMouseX,fMouseY)==true){
            if(g_GameMain.GetGameState()!=5){
                g_GameMain.setZombieState(5);
                g_GameMain.aoerjia->SetSpriteColorAlpha(0);
                g_GameMain.setZombieLastCreateTime(-20);
            }
        }
        //点击铲子
        if(g_GameMain.chanziCao->IsPointInSprite(fMouseX,fMouseY)==true){
            g_GameMain.chanziDong->SetSpriteColorAlpha(100);
            g_GameMain.setMouseCarry(2);
            return ;
        }
    break;
    case 1:
        //放置植物
        if(g_GameMain.isMouseInGrass(fMouseX,fMouseY)){
            int cellX=g_GameMain.getShadowCellX(fMouseX),cellY=g_GameMain.getShadowCellY(fMouseY);
            if(g_GameMain.getCellHasPlant(cellX,cellY) < 0){                                                                             //如果这一格没有植物
                char tmpName [20]="plantAt  ";
                tmpName[7]=(char)(cellX+48);
                tmpName[8]=(char)(cellY+48);
                g_GameMain.cell_plant[cellX][cellY] = new LifeCSprite(tmpName,g_GameMain.plants[g_GameMain.getCardChoose()]->GetName(),
                                                                      cellX,cellY,g_GameMain.getCardChoose());                          //创建这一格的植物
                g_GameMain.cell_plant[cellX][cellY]->SetSpritePosition(g_GameMain.getShadowX(fMouseX),g_GameMain.getShadowY(fMouseY));  //把植物移动到这一格的位置
                g_GameMain.setCellHasPlant(cellX,cellY,g_GameMain.getCardChoose());
                g_GameMain.setMySunLight(g_GameMain.getMySunLight()-g_GameMain.card_plants[g_GameMain.getCardChoose()]->getCost());
                g_GameMain.card_plants[g_GameMain.getCardChoose()]->setLastCreate(nowTime);
                g_GameMain.cardText[g_GameMain.getCardChoose()]->SetTextValue(g_GameMain.card_plants[g_GameMain.getCardChoose()]->getCD());
            }
        }
        if(g_GameMain.card_plants[g_GameMain.getCardChoose()]->getCD() - (nowTime-g_GameMain.card_plants[g_GameMain.getCardChoose()]->getLastCreate()) <= 0){
            g_GameMain.card_plants[g_GameMain.getCardChoose()]->SetSpriteColorRed(255); //恢复卡片颜色
        }
        g_GameMain.hover->DeleteSprite();
        g_GameMain.shadow->DeleteSprite();
        g_GameMain.setCardChoose(-1);
        g_GameMain.setMouseCarry(0);
    break;
    case 2:
        //铲除植物
        if(g_GameMain.isMouseInGrass(fMouseX,fMouseY)){
            int cellX=g_GameMain.getShadowCellX(fMouseX),cellY=g_GameMain.getShadowCellY(fMouseY);
            if(g_GameMain.getCellHasPlant(cellX,cellY) >= 0){
                g_GameMain.cell_plant[cellX][cellY]->DeleteSprite();
                g_GameMain.setCellHasPlant(cellX,cellY,-1);
                float posX=g_GameMain.cell_plant[cellX][cellY]->GetSpritePositionX(),posY=g_GameMain.cell_plant[cellX][cellY]->GetSpritePositionY();
                for(int i=((g_GameMain.getZombieRemain()-6)>0 ? g_GameMain.getZombieRemain()-6 : 0); i<ZOMBIE_SUM; i++){
                    if(g_GameMain.getHasZombie(i)>=0){
                        if(g_GameMain.zombie_body[i][0]->IsPointInSprite(posX-6,posY)||g_GameMain.zombie_body[i][0]->IsPointInSprite(posX-1,posY)||
                           g_GameMain.zombie_body[i][0]->IsPointInSprite(posX+5,posY)||g_GameMain.zombie_body[i][0]->IsPointInSprite(posX+10,posY)){
                                g_GameMain.zombie_body[i][0]->SetSpriteColorAlpha(255);
                                g_GameMain.zombie_body[i][1]->SetSpriteColorAlpha(0);
                                g_GameMain.zombie_body[i][0]->SetSpriteLinearVelocity(-4,0);
                        }
                    }
                }
            }
        }
        g_GameMain.chanziJing->SetSpritePosition(-25,-34);
        g_GameMain.chanziDong->SetSpritePosition(-120,-120);
        g_GameMain.setMouseCarry(0);
    break;
    case 4:
        //给植物使用过载·LBW得得得得得啸
        if(g_GameMain.isMouseInGrass(fMouseX,fMouseY)){
            int cellX=g_GameMain.getShadowCellX(fMouseX),cellY=g_GameMain.getShadowCellY(fMouseY);
            if(g_GameMain.getCellHasPlant(cellX,cellY) >= 0){
                if(g_GameMain.cell_plant[cellX][cellY]->getCardIndex()==2 || g_GameMain.cell_plant[cellX][cellY]->getCardIndex()==4){//仅可以对射手使用
                    g_GameMain.cell_plant[cellX][cellY]->setIsOverload(1);
                    g_GameMain.cell_plant[cellX][cellY]->setOverloadTime(nowTime);
                    g_GameMain.cell_plant[cellX][cellY]->SetSpriteColorBlue(0);
                    g_GameMain.cell_plant[cellX][cellY]->SetSpriteColorGreen(0);
                    g_GameMain.dedede[1]->PlaySoundA();
                    g_GameMain.setLbwUsed(1);
                }
            }
        }
        if(g_GameMain.getLbwUsed()>0){
            g_GameMain.lbwnb->SetSpriteColorAlpha(0);
        }else{
            g_GameMain.lbwnb->SetSpriteColorRed(255);
        }
        g_GameMain.hover->DeleteSprite();
        g_GameMain.shadow->DeleteSprite();
        g_GameMain.setMouseCarry(0);
    break;
    }
}
}
//==========================================================================
//
// 引擎捕捉鼠标弹起消息后，将调用到本函数
void CSystem::OnMouseUp( const int iMouseType, const float fMouseX, const float fMouseY )
{
	// 可以在此添加游戏需要的响应函数

}
//==========================================================================
//
// 引擎捕捉键盘按下消息后，将调用到本函数
// bAltPress bShiftPress bCtrlPress 分别为判断Shift，Alt，Ctrl当前是否也处于按下状态。比如可以判断Ctrl+E组合键
void CSystem::OnKeyDown( const int iKey, const bool bAltPress, const bool bShiftPress, const bool bCtrlPress )
{
	// 可以在此添加游戏需要的响应函数

	//按空格开始游戏
	if(g_GameMain.getGameFocus()==1 && iKey==KEY_SPACE){
        g_GameMain.setStartTime(clock()/CLOCKS_PER_SEC);
        g_GameMain.focusOne[0]->SetSpritePosition(120,-120);
        g_GameMain.SetGameState(1);
	}

}
//==========================================================================
//
// 引擎捕捉键盘弹起消息后，将调用到本函数
void CSystem::OnKeyUp( const int iKey )
{
	// 可以在此添加游戏需要的响应函数

}

//===========================================================================
//
// 引擎捕捉到精灵与精灵碰撞之后，调用此函数
void CSystem::OnSpriteColSprite( const char *szSrcName, const char *szTarName )
{
if(g_GameMain.getGameFocus()==2 || g_GameMain.getGameFocus()==3){
    if(strncmp(szTarName,"plant",5)==0){
        //僵尸与精灵碰撞
        int cellX=(int)szTarName[7]-48,cellY=(int)szTarName[8]-48;
        int zb=((int)szSrcName[6]-48)*10+(int)szSrcName[7]-48;
        if(g_GameMain.getHasZombie(zb)>0){
            g_GameMain.cell_plant[cellX][cellY]->setHp(g_GameMain.cell_plant[cellX][cellY]->getHp()-2);
            g_GameMain.zombie_body[zb][0]->SetSpriteColorAlpha(0);
            g_GameMain.zombie_body[zb][1]->SetSpriteColorAlpha(255);
            g_GameMain.zombie_body[zb][0]->SetSpriteLinearVelocity(-0.1,0);
            if(g_GameMain.cell_plant[cellX][cellY]->getCardIndex()==1){
                if(g_GameMain.cell_plant[cellX][cellY]->getHp()<=1600){             //对坚果墙处理损失1/3血后更换图片
                    char name[20];
                    float posX=g_GameMain.cell_plant[cellX][cellY]->GetSpritePositionX(),posY=g_GameMain.cell_plant[cellX][cellY]->GetSpritePositionY();
                    strcpy(name,g_GameMain.cell_plant[cellX][cellY]->GetName());
                    g_GameMain.cell_plant[cellX][cellY]->DeleteSprite();
                    g_GameMain.cell_plant[cellX][cellY]=new LifeCSprite(name,"wall2",cellX,cellY,101);
                    g_GameMain.cell_plant[cellX][cellY]->SetSpritePosition(posX,posY);
                }
            }else if(g_GameMain.cell_plant[cellX][cellY]->getCardIndex()==101){
                if(g_GameMain.cell_plant[cellX][cellY]->getHp()<=800){              //对坚果墙处理损失2/3血后更换图片
                    char name[20];
                    float posX=g_GameMain.cell_plant[cellX][cellY]->GetSpritePositionX(),posY=g_GameMain.cell_plant[cellX][cellY]->GetSpritePositionY();
                    strcpy(name,g_GameMain.cell_plant[cellX][cellY]->GetName());
                    g_GameMain.cell_plant[cellX][cellY]->DeleteSprite();
                    g_GameMain.cell_plant[cellX][cellY]=new LifeCSprite(name,"wall3",cellX,cellY,102);
                    g_GameMain.cell_plant[cellX][cellY]->SetSpritePosition(posX,posY);
                }
            }
            if(g_GameMain.cell_plant[cellX][cellY]->getHp()<=0){                    //植物的死亡
                float posX=g_GameMain.cell_plant[cellX][cellY]->GetSpritePositionX(),posY=g_GameMain.cell_plant[cellX][cellY]->GetSpritePositionY();
                g_GameMain.cell_plant[cellX][cellY]->DeleteSprite();
                g_GameMain.setCellHasPlant(cellX,cellY,-1);
                for(int i=((g_GameMain.getZombieRemain()-6)>0 ? g_GameMain.getZombieRemain()-6 : 0); i<ZOMBIE_SUM; i++){
                    if(g_GameMain.getHasZombie(i)>=0){                              //恢复僵尸为行走状态
                        if(g_GameMain.zombie_body[i][0]->IsPointInSprite(posX-6,posY)||g_GameMain.zombie_body[i][0]->IsPointInSprite(posX-1,posY)||
                           g_GameMain.zombie_body[i][0]->IsPointInSprite(posX+5,posY)||g_GameMain.zombie_body[i][0]->IsPointInSprite(posX+10,posY)){
                                g_GameMain.zombie_body[i][0]->SetSpriteColorAlpha(255);
                                g_GameMain.zombie_body[i][1]->SetSpriteColorAlpha(0);
                                g_GameMain.zombie_body[i][0]->SetSpriteLinearVelocity(-4,0);
                        }
                    }
                }
            }
        }
    }else if(strncmp(szTarName,"bullet",6)==0){                 //豌豆子弹与僵尸碰撞
        int bulletNumber=((int)szTarName[6]-48)*100+((int)szTarName[7]-48)*10+(int)szTarName[8]-48;
        int zb=((int)szSrcName[6]-48)*10+(int)szSrcName[7]-48;
        g_GameMain.pea[bulletNumber]->DeleteSprite();
        if(g_GameMain.getHasZombie(zb)>0)
            g_GameMain.zombie_body[zb][0]->setHp(g_GameMain.zombie_body[zb][0]->getHp()-100);
        if(g_GameMain.getHasZombie(zb)>0 && g_GameMain.zombie_body[zb][0]->getHasCap()!=0){
            if(g_GameMain.zombie_body[zb][0]->getHp()<=1200){
                g_GameMain.zombie_body[zb][2]->SpriteDismount();
            g_GameMain.zombie_body[zb][2]->SetSpriteLinearVelocity(0.6,2);
            g_GameMain.zombie_body[zb][2]->SetSpriteColorAlpha(200);
            g_GameMain.zombie_body[zb][2]->SetSpriteLifeTime(1);
            g_GameMain.zombie_body[zb][0]->setHasCap(0);
            }
        }
        if(g_GameMain.getHasZombie(zb)>0 && g_GameMain.zombie_body[zb][0]->getHp()<=0){         //僵尸死亡
            g_GameMain.setRowZombieNum(g_GameMain.zombie_body[zb][0]->getCellY(),g_GameMain.getRowZombieNum(g_GameMain.zombie_body[zb][0]->getCellY())-1);
            g_GameMain.zombie_body[zb][0]->DeleteSprite();
            g_GameMain.setHasZombie(zb,-1);
        }
    }else if(strncmp(szTarName,"theleftlimit",12)==0){  //僵尸碰撞边界，游戏结束。由于我的程序里OnSpriteColWorldLimit无法触发，在左边界设置一精灵，Y轴方向铺满，以用检测边界碰撞
        g_GameMain.setGameFocus(4);
        g_GameMain.setFiled(1);
    }
}
}
//===========================================================================
//
// 引擎捕捉到精灵与世界边界碰撞之后，调用此函数.
// iColSide : 0 左边，1 右边，2 上边，3 下边
void CSystem::OnSpriteColWorldLimit( const char *szName, const int iColSide )
{

}

