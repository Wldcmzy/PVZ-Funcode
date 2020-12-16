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
//����ļ�
//����дע��
////////////////////////////////////////////////////////////////////////////////
//
//
CGameMain		g_GameMain;

//==============================================================================
//
// ����ĳ�������Ϊ��GameMainLoop����Ϊ��ѭ��������������ÿ֡ˢ����Ļͼ��֮�󣬶��ᱻ����һ�Ρ�

//==============================================================================
//
// ���캯��
CGameMain::CGameMain()
{
	m_iGameState			=	0;
    setGameFocus(1);
    setGameLevel(0);
    setMusicState(1);

}
//==============================================================================
//
// ��������
CGameMain::~CGameMain()
{
}

//==============================================================================
//
// ��Ϸ��ѭ�����˺���������ͣ�ĵ��ã�����ÿˢ��һ����Ļ���˺�����������һ��
// ���Դ�����Ϸ�Ŀ�ʼ�������С������ȸ���״̬.
// ��������fDeltaTime : �ϴε��ñ��������˴ε��ñ�������ʱ��������λ����
void CGameMain::GameMainLoop( float	fDeltaTime )
{
	switch( GetGameState() )
	{
		// ��ʼ����Ϸ�������һ���������
	case 1:
		{
			GameInit();
			SetGameState(2); // ��ʼ��֮�󣬽���Ϸ״̬����Ϊ������
		}
		break;

		// ��Ϸ�����У����������Ϸ�߼�
	case 2:
		{
			// TODO �޸Ĵ˴���Ϸѭ�������������ȷ��Ϸ�߼�
			if( getGameFocus()==2 || getGameFocus()==3 )
			{
				GameRun( fDeltaTime );
			}
			else // ��Ϸ������������Ϸ���㺯����������Ϸ״̬�޸�Ϊ����״̬
			{
				SetGameState(0);
				GameEnd();
			}
		}
		break;

		// ��Ϸ����/�ȴ����ո����ʼ
	case 0:
	default:
		break;
	};
}
//=============================================================================
//
// ÿ�ֿ�ʼǰ���г�ʼ���������һ���������
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
// ÿ����Ϸ������
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
    //�ı�ˢ��ʬ����
    if(getZombieState()!=5){  //��������������ģʽ
        if(getZombieState()==1 && getZombieRemain()<=ZOMBIE_SUM-5){
            setZombieState(2);
        }else if(getZombieState()==2 && getZombieRemain()<=ZOMBIE_SUM-15){
            setZombieState(3);
        }else if(getZombieState()==3 && getZombieRemain()<=(int)(ZOMBIE_SUM/2)){
            setZombieState(4);
        }
    }
    //ˢ�¿�Ƭ��ȴ
    for(int i=0; i<5; i++){
        int nowCD =card_plants[i]->getCD()-(nowTime-card_plants[i]->getLastCreate());
        if( nowCD > 0){
            cardText[i]->SetTextValue(nowCD);
        }else if( nowCD == 0){
            cardText[i]->SetTextString("��");
            card_plants[i]->SetSpriteColorRed(255);  //�ָ���Ƭ��ɫ
        }else{
            cardText[i]->SetTextChar(' ');
        }
    }
    //ˢ��ʬ
    if(nowTime-getZombieLastCreateTime()>getZombieCreatWaitTime()){
        setZombieLastCreateTime(nowTime);
        for(int i=0; i<getZombieCreatNum() && getZombieRemain()>0; i++){
            int index=rdm(5),pos=rdm(5);//index��ʬ���࣬pos��ʬ���ڵ���
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
            case 1://��ͨ��ʬ
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
            case 2://·�Ͻ�ʬ
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
            case 3://��Ͱ��ʬ
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

    //ֲ�����
    for(int i=0; i<5; i++){
        for(int j=0; j<9; j++){
            if(getCellHasPlant(j,i)>=0){
                if(cell_plant[j][i]->getCardIndex()==4 || cell_plant[j][i]->getCardIndex()==2 ){    //������λ�õ�ֲ��������
                    if(nowTime-cell_plant[j][i]->getLastShoot()>=1){
                        int bulletNum=0;
                        switch(cell_plant[j][i]->getCardIndex())
                        {
                        case 2:     bulletNum=1;    break;      //���η����㶹����Ŀ
                        case 4:     bulletNum=2;    break;
                        }
                        if(getRowZombieNum(i)>0){               //�����һ���н�ʬ���ͷ����㶹
                            for(int k=0; k<bulletNum; k++){
                                char name[20]="bullet   ";
                                name[6]=(char)(getPeaTurn()/100+48);
                                name[7]=(char)((getPeaTurn()/10)%10)+48;
                                name[8]=(char)(getPeaTurn()%10)+48;
                                pea[getPeaTurn()]=new PeaCSprite(name,"pea",getPeaTurn());
                                pea[getPeaTurn()]->SetSpriteLifeTime(2);
                                pea[getPeaTurn()]->SetSpriteLinearVelocity(50,0);
                                pea[getPeaTurn()]->SetSpritePosition(cell_plant[j][i]->GetSpritePositionX()+2*k,cell_plant[j][i]->GetSpritePositionY()-2);
                                setPeaTurn((getPeaTurn()+1)%270);   //���㶹����ָ�������±곬��������ֵ�������±�
                            }
                        }
                        if(cell_plant[j][i]->getIsOverload()>0){    //���ֲ���Ƿ���أ�ʹ����LBW�õõõõõõ�Х��
                            if(nowTime-cell_plant[j][i]->getOverloadTime()>=7){
                                cell_plant[j][i]->DeleteSprite();
                                setCellHasPlant(j,i,-1);
                            }
                        }else{
                            cell_plant[j][i]->setLastShoot(nowTime);    //���û�й��أ����ϴη����㶹��ʱ����Ϊ�����ڡ�
                        }
                    }
                }else if(cell_plant[j][i]->getCardIndex()==0){          //�����һ�������տ�����ʱ��������
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
                }else if(cell_plant[j][i]->getCardIndex()==3){              //�����һ����ӣ��ը��
                    if(nowTime-cell_plant[j][i]->getCreateTime()==1){
                        dedede[0]->PlaySoundA();                //���ű�ը��Ч
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
                                       int zombiePosX=zombie_body[k][0]->GetSpritePositionX(),zombiePosY=zombie_body[k][0]->GetSpritePositionY();//����Щλ�õĽ�ʬ����
                                       int row=zombie_body[k][0]->getCellY();
                                       zombie_body[k][0]->DeleteSprite();
                                       setHasZombie(k,0);
                                       setRowZombieNum(row,getRowZombieNum(row)-1);
                                       char name[15]="ash   ";
                                       name[3]=(char)(k/10)+48;
                                       name[4]=(char)(k%10)+48;
                                       zombie_body[k][3]=new LifeCSprite(name,"zombie_ash",-5,-5,-5);       //��ʬ�ҽ�����
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

    //����ˢ���� and ɾ������ʱ�䲻������⣬��Сbug
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
// ������Ϸ����
void CGameMain::GameEnd()
{
    //��Ҫ����������ͽ���λ�õ��ƶ�
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
//                      <-- �Զ����� -->
//������Ϊ"_hover"�ľ�����Ϊ������λ,����������ƶ�(��CSystem::OnMouseMove��ʹ������λ��������ƶ�)
void CGameMain::creatHover(CSprite *sprite)
{
    hover=new CSprite("_hover",sprite->GetName());
}
//-----------------------------------------------------------------------------------------------------------
//������Ϊ"_shadow"�ľ�����ΪӰ�ӵ�λ,ʹ���ֱ����λ�ķ���λ��(��CSystem::OnMouseMove�п���Ӱ�ӵ�λ�ƶ��ƶ�)
void CGameMain::creatShadow(CSprite *sprite)
{
    shadow=new CSprite("_shadow",sprite->GetName());
    shadow->SetSpriteColorAlpha(100);
}
//-----------------------------------------------------------------------------------------------------
//�������Ƿ��ڲݵط�ΧX>=LINE_LEFT && X<=LINE_RIGHT&&Y>=LINE_TOP && Y<=LINE_BOTTOM֮��
//�Ƿ���1�����Ƿ���0
int CGameMain::isMouseInGrass(const float X,const float Y)
{
    return (X>=LINE_LEFT && X<=LINE_RIGHT && Y>=LINE_TOP && Y<=LINE_BOTTOM) ? 1 : 0 ;
}
//---------------------------------------------------------------------------------------------------------
//����Ӱ��Ӧ�ô��ڵ�XY���꣬����Ӱ�ӵ�λ�ƶ�,��Ҫ��X>=LINE_LEFT && X<=LINE_RIGHT&&Y>=LINE_TOP && Y<=LINE_BOTTOM����������
float CGameMain::getShadowX(const float X)
{
    return ((int)((X-LINE_LEFT)/CELL_WIDTH)*CELL_WIDTH+LINE_LEFT+OFFSET_WIDTH);
}
float CGameMain::getShadowY(const float Y)
{
    return ((int)((Y-LINE_TOP)/CELL_HEIGHT)*CELL_HEIGHT+LINE_TOP+OFFSET_HEIGHT);
}
//----------------------------------------------------------------------------------------------------------
//����Ӱ��Ӧ�ô��ڵ�cellHasPlant����λ��,��Ҫ��X>=LINE_LEFT && X<=LINE_RIGHT&&Y>=LINE_TOP && Y<=LINE_BOTTOM����������
int CGameMain::getShadowCellX(const float X)
{
    return (int)((int)(X-LINE_LEFT)/(CELL_WIDTH));
}
int CGameMain::getShadowCellY(const float Y)
{
    return (int)((int)(Y-LINE_TOP)/CELL_HEIGHT);
}
//----------------------------------------------------------------------------------------------------------
//���ó���ʬ��ʽ
//����ʬ�ķ�ʽ  1��ǰ�� 2������ ,3:���� 4�����һ�� 5���������
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
//����ˢ��ʬ�б�1:��ͨ��2��·�ϣ�3����Ͱ
void CGameMain::setZombieList(int v1,int v2,int v3,int v4,int v5)
{
    zombieList[0]=v1;
    zombieList[1]=v2;
    zombieList[2]=v3;
    zombieList[3]=v4;
    zombieList[4]=v5;

}
//----------------------------------------------------------------------------------------------------------
//���ý�ʬ����ǿ��
//����value=1��ȫ��ͨ�������Ѷ�
//value=2����ͨռ2/5��·��ռ3/5
//value=3����ͨռ1/5��·��ռ2/5����Ͱռ2/5
//value=4��·��ռ3/5����Ͱռ2/5
//value=5��·��ռ1/5����Ͱռ4/5
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
//���ÿ�Ƭ��ȴ
void CGameMain::setCardLastCreateZero()
{
    for(int i=0; i<5; i++){
        card_plants[i]->setLastCreate(-50);
    }
}
//----------------------------------------------------------------------------------------------------------
//ɾ����������һ�ֵľ���
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
//���ÿ�Ƭcd  1���� ��ͨģʽ��cd 2��������ģʽ��cd
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
