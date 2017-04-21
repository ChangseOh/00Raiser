#pragma execution_character_set("utf-8")
#include "GameManager.h"
using namespace cocos2d::experimental;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform\android\jni\JniHelper.h"
#endif

USING_NS_CC;
using namespace std;
using namespace cocos2d::experimental;

#define OPTFILE "gamedata.opt"
#define MAINFILE "gamedata.dat"

Size visibleSize;
Vec2 center;

ValueMap strs;
int _audioID;

int gold;//나뭇잎
int weapon;//장풍단계
int level;//장풍레벨
int fruits;//열매 소지 갯수
int potionA;//포션1 소지 갯수
int potionB;//포션2 소지 갯수
float critical;//크리티컬 (0이면 크리티컬 없다. 이후 데미지값)
float fruitsTime;//열매 0이면 열매 생성 안함. 이후는 열매 유효시간
float autoTime;//자동
float multiple;//분신. 0이면 없다. 이후는 발사 간격 시간(=발사속도)
int wakeup;//각성. 0이면 각성 조건 없음. 이후는 각성에 필요한 연타 수
float dropBonus;//드롭량 증가


//달성기록
int touchNum;//터치 횟수
int shootNum;//실제 발사 수
int maxItem;//최대 열매 소지 수
int viewtimeCm;//리워드 동영상 광고를 시청한 횟수
tm playtime;//게임 켜놓은 시간


//공용 플래그
bool hasNew;//장풍 단계를 높일 수 있다.
bool hasUpgrade;//뭔가 업그레이드 할 수 있는 것이 있다
bool hasReward;//보상받을 수 있는 것이 있다.

char codeKey[] = "raisegame#1923318";
bool soundOnOff;

tm adtime;

Vec2 getDest(Vec2 pos1, int angle, float speed)
{

	float x = speed * sin(angle * 3.14159f / 180.0f);
	float y = speed * cos(angle * 3.14159f / 180.0f);

	return Vec2(pos1.x + x, pos1.y + y);
}
int getAngle(Vec2 pos1, Vec2 pos2)
{

	float vx = pos2.x - pos1.x;
	float vy = pos2.y - pos1.y;
	double rad = atan2(vx, vy);
	int degree = (int)((rad * 180) / 3.1415f);
	return (degree + 360) % 360;
}
//int needGold(int level)
//{
//	int a = 1;
//	//int b = 0;
//	for (int i = 0; i < level; i++)
//	{
//		a += ((int)(pow(2, (i % 5)) * i));
//	}
//
//	return a;
//}
int getValue(int raw)
{
	char temp[4];
	temp[0] = (char)(raw >> 24);
	temp[1] = (char)(raw >> 16);
	temp[2] = (char)(raw >> 8);
	temp[3] = (char)(raw);

	for (int i = 0; i<4; i++)
		temp[i] ^= codeKey[i%sizeof(codeKey)];

	return (
		((temp[0] & 0xff) << 24) +
		((temp[1] & 0xff) << 16) +
		((temp[2] & 0xff) << 8) +
		(temp[3] & 0xff)
		);
}
string numComma(int num)
{

	char _str[30];

	sprintf(_str, "%d", num);
	string _strtemp = string(_str);

	int nPos = _strtemp.length();
	if (_strtemp.length()>3) {
		while (nPos>3)
			_strtemp.insert(nPos -= 3, ",");
	}

	return _strtemp;
}
void preLoadEffect()
{

	//string fnames[17] = {
	//	"sound/booboo.mp3",
	//	"sound/damage_dog.mp3",
	//	"sound/damage_female.mp3",
	//	"sound/damage_male.mp3",
	//	"sound/effect_06.mp3",
	//	"sound/effect_06r.mp3",
	//	"sound/effect_08.mp3",
	//	"sound/effect_14.mp3",
	//	"sound/effect_15.mp3",
	//	"sound/se_cannon.mp3",
	//	"sound/se_cash.mp3",
	//	"sound/se_coin.mp3",
	//	"sound/whig.mp3",
	//	"sound/whistle.mp3",
	//	"sound/windshoot.mp3",
	//	"sound/spc1000_O8O4ting.mp3",
	//	"sound/star.mp3"
	//};
	//for (int i = 0; i < 17; i++)
	//{
	//	//AudioEngine::play2d(fnames[i], false, 0.0f);
	//	AudioEngine::preload(fnames[i]);
	//	//audioEngine->preloadEffect(fnames[i].c_str());
	//}
}
void playEffect(const char* fname)
{

	if (!soundOnOff)
		return;

	AudioEngine::play2d(fname, false, 1.0f);
	//audioEngine->setEffectsVolume(1.0f);
	//audioEngine->playEffect(fname);
}
void playEffect(int kind)
{

	if (!soundOnOff)
		return;

	string fnames[15] = {
		"sound/booboo.mp3",
		"sound/damage_dog.mp3",
		"sound/damage_female.mp3",
		"sound/damage_male.mp3",
		"sound/effect_06.mp3",
		"sound/effect_06r.mp3",
		"sound/effect_08.mp3",
		"sound/effect_14.mp3",
		"sound/effect_15.mp3",
		"sound/se_cannon.mp3",
		"sound/se_cash.mp3",
		"sound/se_coin.mp3",
		"sound/whig.mp3",
		"sound/whistle.mp3",
		"sound/windshoot.mp3"
	};

	AudioEngine::play2d(fnames[kind], false, 1.0f);
	//audioEngine->setEffectsVolume(1.0f);
	//audioEngine->playEffect(fnames[kind].c_str());
}
void playMusic(const char* fname)
{
	//audioEngine->setBackgroundMusicVolume(0.8f);
	//audioEngine->playBackgroundMusic(fname, true);
	if (!soundOnOff)
		return;

	if (_audioID == AudioEngine::INVALID_AUDIO_ID) {
		_audioID = AudioEngine::play2d(fname, true, 0.6f);
	}
	else
	{
		stopMusic();
		_audioID = AudioEngine::play2d(fname, true, 0.6f);
	}

	if (!soundOnOff)
		AudioEngine::pauseAll();
		//audioEngine->pauseBackgroundMusic();
}
void stopMusic()
{
	if (_audioID != AudioEngine::INVALID_AUDIO_ID)
	{
		AudioEngine::stop(_audioID);
		_audioID = AudioEngine::INVALID_AUDIO_ID;
	}
}
void loadOption()
{
	auto futil = FileUtils::getInstance();
	string temptilepath = futil->getWritablePath().append(OPTFILE);

	if (!futil->isFileExist(temptilepath))
	{
		soundOnOff = true;
		saveOption();
		return;
	}

	ValueMap optionData;
	optionData = futil->getValueMapFromFile(temptilepath);
	soundOnOff = optionData["SOUND"].asBool();

	futil->destroyInstance();
}
void saveOption()
{
	ValueMap optionData;
	optionData["SOUND"] = soundOnOff;

	auto futil = FileUtils::getInstance();
	string temptilepath = futil->getWritablePath().append(OPTFILE);
	futil->writeValueMapToFile(optionData, temptilepath);

	futil->destroyInstance();
}

bool loadGame()
{

	//바이너리를 읽는다
	Data data;
	ssize_t fsize;
	string filePath;
	auto futil = FileUtils::getInstance();
	string temptilepath = futil->getWritablePath().append("tempdata.tmp");
	if (futil->isFileExist(temptilepath))
	{
		//Google Cloud에서 읽어온 파일로 대체한다
		//CCLOG("load from temp");
		fsize = futil->getFileSize(temptilepath);
		if (fsize == 0)
			return false;

		data = futil->getDataFromFile(temptilepath);

		//읽었으므로 곧바로 삭제한다
		//클라우드에서 받아온 파일이 뻑나있다면 디코딩된 파일을 파싱할 때 앱이 죽는다
		//템프파일은 이미 삭제됐으므로 다시 실행하면 일단 정상 실행 가능하고, 다시 클라우드에서 다운받으면 된다.
		futil->removeFile(temptilepath);
	}
	else
	{
		//CCLOG("load from local");
		filePath = futil->getWritablePath().append(MAINFILE);

		if (!futil->isFileExist(filePath))
			return false;
		fsize = futil->getFileSize(filePath);
		if (fsize == 0)
			return false;

		data = futil->getDataFromFile(filePath);
	}
	unsigned char* szTmp = data.getBytes();

	//읽은 바이너리를 디코딩한다
	for (int i = 0; i < fsize; i++)
		szTmp[i] = (szTmp[i] ^ codeKey[i % sizeof(codeKey)]);

	//디코딩된 데이타로 temp파일을 생성한다
	filePath = futil->getWritablePath().append("tempdata.db");
	//Data d;
	//d.fastSet(szTmp, fsize);
	//bool ret = futil->writeDataToFile(d, filePath);
	//d.fastSet(nullptr, 0);
	//if (!ret)
	//	return false;

//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//	wstring wstr = wstring(filePath.begin(), filePath.end());
//	wstring md(L"wb");
//	auto fp = _wfopen(wstr.c_str(), md.c_str());
//	if (fp != NULL)
//	{
//		fwrite(szTmp, sizeof(char), fsize, fp);
//		fclose(fp);
//	}
//	else return false;
//#else
	auto fp = fopen(futil->getSuitableFOpen(filePath).c_str(), "wb");
	if (fp != NULL)
	{
		fwrite(szTmp, sizeof(char), fsize, fp);
		fclose(fp);
	}
	else return false;
//#endif

	//temp파일을 ValueMap 형태로 읽어들인다
	ValueMap gameData = futil->getValueMapFromFile(filePath);

	//ValueMap을 파싱한다
	//ValueMap을 파싱한다
	//ValueMap을 파싱한다
	//ValueMap을 파싱한다
	//ValueMap을 파싱한다

	//temp파일을 삭제한다
	futil->removeFile(filePath);
	futil->destroyInstance();

	return true;
}
void saveGame()
{
	ValueMap saveData;
	//ValueMap을 파싱한다
	//ValueMap을 파싱한다
	//ValueMap을 파싱한다
	//ValueMap을 파싱한다


	auto futil = FileUtils::getInstance();

	//ValueMap 저장 (Parsed XML type)
	string filePath = futil->getWritablePath().append("tempdata.db");
	futil->writeValueMapToFile(saveData, filePath);

	//ValueMap 형태의 temp파일을 다시 바이너리로 읽어들인다
	auto data = futil->getDataFromFile(filePath);
	ssize_t fsize = data.getSize();//futil->getFileSize(filePath);
	unsigned char* temp = data.getBytes();

	//읽어들인 바이너리를 코드화해서 저장한다
	for (int i = 0; i < fsize; i++)
		temp[i] = (temp[i] ^ codeKey[i % sizeof(codeKey)]);
	filePath = futil->getWritablePath().append(MAINFILE);
	//Data d;
	//d.fastSet(temp, fsize);
	//bool ret = futil->writeDataToFile(d, MAINFILE);
	//d.fastSet(nullptr, 0);
	//if (!ret)
	//	CCLOG("save failed");

	auto fp = fopen(futil->getSuitableFOpen(filePath).c_str(), "wb");
	if (fp != NULL)
	{
		fwrite(temp, sizeof(char), fsize, fp);
		fclose(fp);
	}
	//else
	//{
	//	wstring wstr = L"";
	//	CCLOG("%s", filePath.c_str());
	//	wstr.assign(filePath.begin(), filePath.end());
	//	auto fp = _wfopen(wstr.c_str(), L"wb");
	//	if (fp != NULL)
	//	{
	//		fwrite(temp, sizeof(char), fsize, fp);
	//		fclose(fp);
	//	}
	//}

	//temp파일을 삭제한다
	filePath = futil->getWritablePath().append("tempdata.db");
	futil->removeFile(filePath);

	futil->destroyInstance();

}
Vector<Node*> getChildrenByName(Node* node, string name)
{
	Vector<Node*> _ret;
	_ret.clear();
	Vector<Node*> _list = node->getChildren();
	for (int i = 0; i < _list.size(); i++)
	{
		if (_list.at(i)->getName().compare(name) == 0)
			_ret.pushBack(_list.at(i));
	}

	return _ret;
}
cocos2d::Sprite* hitEffect(std::string fname, cocos2d::Vec2 pos)
{
	auto efct = Sprite::create(fname);
	efct->getTexture()->setAliasTexParameters();
	efct->setPosition(pos);
	efct->runAction(Sequence::create(
		ScaleTo::create(0.1f, 1.2f),
		ScaleTo::create(0.1f, 0),
		CallFunc::create([=](void) {
		efct->removeFromParent();
	}),
		NULL));

	return efct;
}
cocos2d::Sprite* hitEffect(std::string fname, cocos2d::Vec2 pos, float scale)
{
	auto efct = Sprite::create(fname);
	efct->getTexture()->setAliasTexParameters();
	efct->setPosition(pos);
	efct->setScale(scale);
	efct->runAction(Sequence::create(
		ScaleBy::create(0.1f, 1.2f),
		ScaleTo::create(0.1f, 0),
		CallFunc::create([=](void) {
		efct->removeFromParent();
	}),
		NULL));

	return efct;
}
double getRange(Vec2 pos1, Vec2 pos2)
{

	return sqrt(abs((pos2.y - pos1.y)*(pos2.y - pos1.y) + (pos2.x - pos1.x)*(pos2.x - pos1.x)));
}

void makeStrJson()
{
	ValueMap strings;
	strings["PROFILE_0"] = "이름:\n사쿠라히메\n\n나이:17세\n\n누명을 쓰고 쫓겨난 영주의 딸.";
	strings["PROFILE_1"] = "무술에 관심이 많아\n어렸을적부터 인술을 수련했다.";
	strings["PROFILE_2"] = "음모를 꾸민 도적단\n'달그림자'에 맞서,\n아버지의 누명을 벗기기 위해 싸운다.";
	strings["PROFILE_3"] = "'모두들, 응원 고마워.\n힘낼게!'";
	strings["PROFILE_4"] = "presents by\n\nG GAME (C) 2017";

	strings["REBIRTH_0"] = "환영합니다.\n작지만 용감한 영혼이여.";
	strings["REBIRTH_1"] = "그대는 훌륭한 잠재능력을 갖고 있군요.";
	strings["REBIRTH_2"] = "하지만 그걸 깨우기 위해서는 모든것을 처음으로 되돌려야 합니다.";
	strings["REBIRTH_3"] = "되돌리기에는 아직 부족하군요. 자격을 갖추고 다시 찾아주길 기다리고 있겠습니다.";
	strings["REBIRTH_4"] = "싸움도, 모은 금화도. 모두 처음부터.\n그 댓가로 얻게 되는 것은 ";
	strings["REBIRTH_5"] = "개의 윤회석.";
	strings["REBIRTH_6"] = "이것으로 당신의 잠재력을 개방할 수 있습니다.";
	strings["REBIRTH_7"] = "선택하세요, 닌자여.";
	strings["REBIRTH_8"] = "그대의 용기에 빛이 함께하길.";

	strings["SHOP_0"] = "두 개의 표창을 나란히 던집니다.";
	strings["SHOP_1"] = "세 방향으로 표창을 던집니다.";
	strings["SHOP_2"] = "세 방향으로 화염탄을 던집니다.";
	strings["SHOP_3"] = "세 방향으로 광륜을 던집니다.";
	strings["SHOP_4"] = "150거리 내의 아이템을 끌어당깁니다.";
	strings["SHOP_5"] = "300거리 내의 아이템을 끌어당깁니다.";
	strings["SHOP_6"] = "수행닌자 쿠로코를 동행합니다.";
	strings["SHOP_7"] = "수행닌자 코가를 동행합니다.";
	strings["SHOP_8"] = "수행닌자 쿠노이치를 동행합니다.";
	strings["SHOP_9"] = "수행닌자 쿠로가네를 동행합니다.";

	strings["ENFORCE_0"] = "표창의 공격력이 강해집니다.";
	strings["ENFORCE_1"] = "공주의 체력이 강해집니다.";
	strings["ENFORCE_2"] = "두루마리의 유효시간이 길어집니다.";

	strings["MESSAGE_0"] = "누르고 있으면 올라갑니다.";
	strings["MESSAGE_1"] = "최고레벨입니다.";
	strings["MESSAGE_2"] = "금화로 아이템을 쓸 수 있습니다.";
	strings["MESSAGE_3"] = "최악의 난이도, 달그림자성에 도전합니다.";
	strings["MESSAGE_4"] = "달그림자성의 레벨은 리셋되지 않습니다.";

	FileUtils::getInstance()->writeValueMapToFile(strings, "kor.json");
	//-----------------------------------------------------------------
	strings.clear();
	strings["PROFILE_0"] = "名前:\nさくらひめ\n\n年齢:17才\n\n濡れ衣を着せられ追放された領主の娘。";
	strings["PROFILE_1"] = "武術に関心が高く\n小さい頃から忍術を修練した。";
	strings["PROFILE_2"] = "陰謀を企て盗賊団「月の影」に対抗して、父の濡れ衣をむきために戦う。";
	strings["PROFILE_3"] = "'みんな、応援ありがとう。さくら頑張る！'";
	strings["PROFILE_4"] = "presents by\n\nG GAME (C) 2017";

	strings["REBIRTH_0"] = "歓迎します。小さいながらも勇敢な魂よ。";
	strings["REBIRTH_1"] = "君は素晴らしい潜在能力を持っていますね。";
	strings["REBIRTH_2"] = "しかし、それを覚醒にすべてを初めてに戻す必要があります。";
	strings["REBIRTH_3"] = "元に戻すには、まだ不足ですね。資格を備えて再度訪問してくれることを待っております。";
	strings["REBIRTH_4"] = "戦いも、集めた金貨も。すべて最初から。\nその代価として得られるのは、";
	strings["REBIRTH_5"] = "個の輪廻の石。";
	strings["REBIRTH_6"] = "これであなたの可能性を開放することができます。";
	strings["REBIRTH_7"] = "選べ、忍者よ。";
	strings["REBIRTH_8"] = "あなたの勇気に光があることを。";

	strings["SHOP_0"] = "二つの手裏剣を並べ投げます。";
	strings["SHOP_1"] = "才方向に手裏剣を投げます。";
	strings["SHOP_2"] = "才方向に火炎弾を投げます。";
	strings["SHOP_3"] = "才方向に光輪を投げます。";
	strings["SHOP_4"] = "150通り内のアイテムを引っ張ります。";
	strings["SHOP_5"] = "300通り内のアイテムを引っ張ります。";
	strings["SHOP_6"] = "傭兵忍者、くろこが同行します。";
	strings["SHOP_7"] = "傭兵忍者、こうがが同行します。";
	strings["SHOP_8"] = "傭兵忍者、くノ一が同行します。";
	strings["SHOP_9"] = "傭兵忍者、鉄が同行します。";

	strings["ENFORCE_0"] = "手裏剣の攻撃力が強くなります。";
	strings["ENFORCE_1"] = "さくらの体力が強くなります。";
	strings["ENFORCE_2"] = "忍術の有効時間が長くなります。";

	strings["MESSAGE_0"] = "押していると上がります。";
	strings["MESSAGE_1"] = "最高レベルです";
	strings["MESSAGE_2"] = "金貨でアイテムを使うことができます。";
	strings["MESSAGE_3"] = "最悪の難易度、月影城に挑戦します。";
	strings["MESSAGE_4"] = "月影城のレベルはリセットされません。";

	FileUtils::getInstance()->writeValueMapToFile(strings, "jpn.json");
	//-----------------------------------------------------------------
	strings.clear();
	strings["PROFILE_0"] = "NAME:\nSAKURAHIME\n\nAGE:17\n\nDaughter of\nformer lord\nwho has been framed.";
	strings["PROFILE_1"] = "She is nterested\n'NINJITSU',\n\nhas been training\nsince was\na very young.";
	strings["PROFILE_2"] = "She fight against\n'Moon Shadows'\nwho put the\nblame,\n\nfor prove\nfarther's innocence.";
	strings["PROFILE_3"] = "'Everyone,\nThanks for Cheering!,\n\nI will do my best'";
	strings["PROFILE_4"] = "presents by\n\nG GAME (C) 2017";

	strings["REBIRTH_0"] = "Welcome,\na small but brave soul";
	strings["REBIRTH_1"] = "If you return to\nlevel 1,\nyou can open\nyour potential";
	strings["REBIRTH_2"] = "If you accept\nthe reincarnation now,\neverything goes back";
	strings["REBIRTH_3"] = "But, you have\nnot enough levels.\nLet's meet again\nif you quality";
	strings["REBIRTH_4"] = "also gold coins.\nbut you can get\n";
	strings["REBIRTH_5"] = "\nreincarnation stones.";
	strings["REBIRTH_6"] = "You can wake up\nyour potential\nwith this stone.";
	strings["REBIRTH_7"] = "Choose,\nyou the Ninja";
	strings["REBIRTH_8"] = "Honor for\nyour courage.\n\nGOOD LUCK";

	strings["SHOP_0"] = "Shuiriken dual shot";
	strings["SHOP_1"] = "Shuriken 3-way shot";
	strings["SHOP_2"] = "Fireball 3-way shot";
	strings["SHOP_3"] = "Chakram 3-way shot";
	strings["SHOP_4"] = "auto catch in 150 range";
	strings["SHOP_5"] = "auto catch in 300 range";
	strings["SHOP_6"] = "use KUROKO as servant";
	strings["SHOP_7"] = "use KOGA as servant";
	strings["SHOP_8"] = "use KUNOICH as servant";
	strings["SHOP_9"] = "use KUROGANE as servant";

	strings["ENFORCE_0"] = "Shuriken becomes strong";
	strings["ENFORCE_1"] = "physical becomes strong";
	strings["ENFORCE_2"] = "Effective time will be longer";

	strings["MESSAGE_0"] = "can hold down rapid go up";
	strings["MESSAGE_1"] = "LEVEL MAX (can't upgrade)";
	strings["MESSAGE_2"] = "can use items in gold coins.";
	strings["MESSAGE_3"] = "Challenge Moon Shadow Castle.";
	strings["MESSAGE_4"] = "Castle level is not reset.";

	FileUtils::getInstance()->writeValueMapToFile(strings, "eng.json");

}
//string codedStr(string src)
//{
//	string cc = "";
//	for (int i = 0; i < src.size(); i++)
//	{
//		unsigned char aa = src.at(i);
//		aa = 256 - aa;
//		cc += aa;
//	}
//
//	return cc;
//	//return src;
//}




#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
static jclass class_String;
static jmethodID mid_newString, mid_newStringEncoding;
jbyteArray cstr2jbyteArray(JNIEnv *env, const char *nativeStr) {
	jbyteArray javaBytes;
	int len = strlen(nativeStr);
	javaBytes = env->NewByteArray(len);
	env->SetByteArrayRegion(javaBytes, 0, len, (jbyte *)nativeStr);

	return javaBytes;
}
jstring javaNewString(JNIEnv *env, jbyteArray javaBytes) {
	jclass cls = env->FindClass("java/lang/String");
	class_String = (jclass)env->NewGlobalRef(cls);
	env->DeleteLocalRef(cls);
	mid_newString = env->GetMethodID(class_String, "<init>", "([B)V");
	return (jstring)env->NewObject(class_String, mid_newString, javaBytes);
}
jstring javaNewStringChar(JNIEnv* env, const char* nativeStr) {
	jbyteArray byteArray = cstr2jbyteArray(env, nativeStr);
	jstring jstr = javaNewString(env, byteArray);
	env->DeleteLocalRef(byteArray);

	return jstr;
}
#endif
RenderTexture* getHotShot()
{
	auto director = Director::getInstance();
	Size size = director->getWinSize();
	RenderTexture* texture = RenderTexture::create((int)size.width, (int)size.height, kCCTexture2DPixelFormat_RGBA8888);
	//RenderTexture* texture = RenderTexture::create(size.width, size.height, kCCTexture2DPixelFormat_RGBA4444);
	texture->setPosition(Vec2(size.width / 2, size.height / 2));
	texture->begin();
	director->getRunningScene()->visit();
	texture->end();

	return texture;
}
bool getScreenShot()
{

	char txt[100];

	auto texture = getHotShot();
	sprintf(txt, "screenshot.jpg");
	bool result = texture->saveToFile(txt, kCCImageFormatJPEG);

	return result;
}
void java_INITAD()
{
	if (true)
		return;
	string ID1 = "5895082846907398";//전면광고ID ca-app-pub-5895082846907398/1148594066 
	string ID11 = "1148594066";
	string ID2 = "9756986060";//배너광고ID ca-app-pub-5895082846907398/9756986060 
	string ID21 = "5895082846907398";

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;
	bool isHave = JniHelper::getStaticMethodInfo(t,
		"org/cocos2dx/cpp/AppActivity",
		"getInstance",
		"()Ljava/lang/Object;");

	jobject act = t.env->CallStaticObjectMethod(t.classID, t.methodID);
	jstring jstrValue = javaNewStringChar(t.env, ID1.c_str());
	jstring jstrValue2 = javaNewStringChar(t.env, ID11.c_str());
	jstring jstrValue3 = javaNewStringChar(t.env, ID2.c_str());
	jstring jstrValue4 = javaNewStringChar(t.env, ID21.c_str());

	if (isHave) {

		if (JniHelper::getMethodInfo(t,
			"org/cocos2dx/cpp/AppActivity",
			"ADInit",
			"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")) {
			t.env->CallVoidMethod(act, t.methodID, jstrValue, jstrValue2, jstrValue3, jstrValue4);
			t.env->DeleteLocalRef(t.classID);
		}
	}
#else
#endif
}
void java_VIEWAD()
{

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;
	bool isHave = JniHelper::getStaticMethodInfo(t,
		"org/cocos2dx/cpp/AppActivity",
		"getInstance",
		"()Ljava/lang/Object;");

	jobject act = t.env->CallStaticObjectMethod(t.classID, t.methodID);

	if (isHave) {

		if (JniHelper::getMethodInfo(t,
			"org/cocos2dx/cpp/AppActivity",
			"showAd",
			"()V")) {
			t.env->CallVoidMethod(act, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}
	}
#else
#endif
}
void java_HIDEAD()
{

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;
	bool isHave = JniHelper::getStaticMethodInfo(t,
		"org/cocos2dx/cpp/AppActivity",
		"getInstance",
		"()Ljava/lang/Object;");

	jobject act = t.env->CallStaticObjectMethod(t.classID, t.methodID);

	if (isHave) {

		if (JniHelper::getMethodInfo(t,
			"org/cocos2dx/cpp/AppActivity",
			"hideAd",
			"()V")) {
			t.env->CallVoidMethod(act, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}
	}
#else
#endif
}
void java_VIEWINTERSTIALAD()
{

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;
	bool isHave = JniHelper::getStaticMethodInfo(t,
		"org/cocos2dx/cpp/AppActivity",
		"getInstance",
		"()Ljava/lang/Object;");

	jobject act = t.env->CallStaticObjectMethod(t.classID, t.methodID);

	if (isHave) {

		if (JniHelper::getMethodInfo(t,
			"org/cocos2dx/cpp/AppActivity",
			"showInterstital",
			"()V")) {
			t.env->CallVoidMethod(act, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}
	}
#else
#endif
}
void java_ACHIEVE(int idx)
{
	// idx==-1이면 목록보기
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;
	bool isHave = JniHelper::getStaticMethodInfo(t,
		"org/cocos2dx/cpp/AppActivity",
		"getInstance",
		"()Ljava/lang/Object;");

	jobject act = t.env->CallStaticObjectMethod(t.classID, t.methodID);

	if (isHave) {

		if (idx == -1)
		{
			if (JniHelper::getMethodInfo(t,
				"org/cocos2dx/cpp/AppActivity",
				"GameAchieve",
				"()V")) {
				t.env->CallVoidMethod(act, t.methodID);
				t.env->DeleteLocalRef(t.classID);
			}
		}
		else
		{
			if (JniHelper::getMethodInfo(t,
				"org/cocos2dx/cpp/AppActivity",
				"SetAchieve",
				"(I)V")) {
				t.env->CallVoidMethod(act, t.methodID, idx);
				t.env->DeleteLocalRef(t.classID);
			}
		}
	}
#else
#endif
}
void java_LEADERBOARD(int kind, int score)
{

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;
	bool isHave = JniHelper::getStaticMethodInfo(t,
		"org/cocos2dx/cpp/AppActivity",
		"getInstance",
		"()Ljava/lang/Object;");

	jobject act = t.env->CallStaticObjectMethod(t.classID, t.methodID);

	if (isHave) {

		if (JniHelper::getMethodInfo(t,
			"org/cocos2dx/cpp/AppActivity",
			"GameLeaderboard",
			"(II)V")) {
			t.env->CallVoidMethod(act, t.methodID, kind, score);
			t.env->DeleteLocalRef(t.classID);
		}
	}
#else
#endif
}
void java_MOREGAME(int kind)
{

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;
	bool isHave = JniHelper::getStaticMethodInfo(t,
		"org/cocos2dx/cpp/AppActivity",
		"getInstance",
		"()Ljava/lang/Object;");

	jobject act = t.env->CallStaticObjectMethod(t.classID, t.methodID);

	if (isHave) {

		if (JniHelper::getMethodInfo(t,
			"org/cocos2dx/cpp/AppActivity",
			"MoreGames",
			"(I)V")) {
			t.env->CallVoidMethod(act, t.methodID, kind);
			t.env->DeleteLocalRef(t.classID);
		}
	}
#else
#endif
}
void java_SAVECLOUD()
{

	auto futil = FileUtils::getInstance();
	string filePath = futil->getWritablePath();// .append("tempdata.db");
	string fileName = MAINFILE;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;
	bool isHave = JniHelper::getStaticMethodInfo(t,
		"org/cocos2dx/cpp/AppActivity",
		"getInstance",
		"()Ljava/lang/Object;");

	jobject act = t.env->CallStaticObjectMethod(t.classID, t.methodID);
	jstring jstrValue = javaNewStringChar(t.env, filePath.c_str());
	jstring jstrValue2 = javaNewStringChar(t.env, fileName.c_str());

	if (isHave) {

		if (JniHelper::getMethodInfo(t,
			"org/cocos2dx/cpp/AppActivity",
			"CloudSave",
			"(Ljava/lang/String;Ljava/lang/String;)V")) {
			t.env->CallVoidMethod(act, t.methodID, jstrValue, jstrValue2);
			t.env->DeleteLocalRef(t.classID);
		}
	}
#else
#endif
}
