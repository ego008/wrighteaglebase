#ifndef __DynamicDebug_H__
#define __DynamicDebug_H__


#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "Observer.h"
#include "PlayerParam.h"

/**
 * ��̬�����м�¼��Ϣ������
 * Message type in dynamic debugging.
 */
enum MessageType
{
    MT_Null,
    MT_Parse,
    MT_Run,
    MT_Send
};

class DynamicDebug
{
	DynamicDebug();

	struct MessageFileHead
	{
		//char    mHeadFlag[2]; // ����ʶ���Ƿ��Ƕ�̬���Լ�¼����Ϣ�ļ�
		Time    mMaxCycle; // �ļ��м�¼���������
		long long  mIndexTableSize; // ��������С
		long long  mIndexTableOffset; // ������λ��
		long long  mParserTableSize;
		long long  mParserTableOffset;
		long long  mDecisionTableSize;
		long long  mDecisionTableOffset;
		long long  mCommandSendTableSize;
		long long  mCommandSendTableOffset;
		/*int  mIndexTableSize; // ��������С
		int  mIndexTableOffset; // ������λ��
		int  mParserTableSize;
		int  mParserTableOffset;
		int  mDecisionTableSize;
		int  mDecisionTableOffset;
		int  mCommandSendTableSize;
		int  mCommandSendTableOffset;*/
		MessageFileHead(): mMaxCycle( -10 )
		{

		}
	};

	struct MessageIndexTableUnit
	{
		Time    mServerTime; // ���ݶ�Ӧ������
		long long  mDataSize; // ���ݳ���
		long long  mDataOffset; // ���ݵĴ洢λ��
		long long  mTimeOffset; // ʱ����Ĵ洢λ��
		/*int  mDataSize; // ���ݳ���
		int  mDataOffset; // ���ݵĴ洢λ��
		int  mTimeOffset; // ʱ����Ĵ洢λ��*/
	};

	struct Message
	{
		MessageType mType;
		std::string mString;

		Message(MessageType type, const char *msg): mType(type), mString(msg)
		{

		}
	};

public:
	/**
     * ���캯������������
     */
    ~DynamicDebug();

    /**
     * ����ʵ������ʼ������
     */
    static DynamicDebug & instance();
    void Initial(Observer *pObserver);

    /**
     * ���溯������������Ҫ��¼server��Ϣʱ�õ��Ľӿ�
     * AddMessage()�������һ��Ҫ�ŵ������̵߳���ں�����ǰ����ã�����̬���Եõ�ϵͳʱ��ʱ�����
     * ���磬Ҫ��Parser::Parse()��Client::Decision()��CommandSend::Run()֮ǰ����
     */
    void AddMessage(const char *msg, MessageType msg_type);
    void AddTimeParser(timeval &time);
    void AddTimeDecision(timeval &time);
    void AddTimeCommandSend(timeval &time);

    /**
     * ���溯���Ƕ�̬����ʱ�õ��Ľӿ�
     */
    MessageType Run(char *msg);
    MessageType GetMessage(char *msg);
    bool FindCycle(int cycle);
    timeval GetTimeParser();
    timeval GetTimeDecision();
    timeval GetTimeCommandSend();

private:
    void Flush();

private:
    Observer    *mpObserver; // WorldModel��ָ��
    bool        mInitialOK; // �Ƿ��Ѿ���ʼ�����

    // �ļ�ͷ����Ϣ����¼4����Ϣ����
    MessageFileHead mFileHead;

    // ����4����������¼�ͱ���4����Ϣ
    std::vector<MessageIndexTableUnit>  mIndexTable;
    std::vector<timeval>                mParserTimeTable;
    std::vector<timeval>                mDecisionTimeTable;
    std::vector<timeval>                mCommandSendTimeTable;

    std::vector<Message>                mMessageTable;

    // ����4��ָ�������ڶ�д�ļ�ʱʹ��
    MessageIndexTableUnit   *mpIndex;
    timeval                 *mpParserTime;
    timeval                 *mpDecisionTime;
    timeval                 *mpCommandSendTime;

    // ��ǰ��ȡ�ĵ�Ԫ
    MessageIndexTableUnit   *mpCurrentIndex;

    // �����ļ�����
    ThreadMutex     mFileMutex;
    FILE            *mpFile;
    std::ifstream   *mpFileStream;
    std::streambuf  *mpStreamBuffer;

    bool mRunning; // �Ƿ���������
    bool mShowMessage; // �Ƿ������Ϣ
    Time mRuntoCycle; // ҪRun��������
};

#endif
