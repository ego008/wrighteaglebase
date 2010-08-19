/************************************************************************************
 * WrightEagle (Soccer Simulation League 2D)                                        *
 * BASE SOURCE CODE RELEASE 2009                                                    *
 * Copyright (C) 1998-2009 WrightEagle 2D Soccer Simulation Team,                   *
 *                         Multi-Agent Systems Lab.,                                *
 *                         School of Computer Science and Technology,               *
 *                         University of Science and Technology of China, China.    *
 *                                                                                  *
 * This program is free software; you can redistribute it and/or                    *
 * modify it under the terms of the GNU General Public License                      *
 * as published by the Free Software Foundation; either version 2                   *
 * of the License, or (at your option) any later version.                           *
 *                                                                                  *
 * This program is distributed in the hope that it will be useful,                  *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of                   *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                    *
 * GNU General Public License for more details.                                     *
 *                                                                                  *
 * You should have received a copy of the GNU General Public License                *
 * along with this program; if not, write to the Free Software                      *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.  *
 ************************************************************************************/

#ifndef __Dasher_H__
#define __Dasher_H__

#include "Geometry.h"
#include "Agent.h"

struct AtomicAction;
class PlayerState;

class Dasher
{
    Dasher();

public:
    ~Dasher();

    static Dasher & instance();

    static Array<double, 8> DASH_DIR;
    static Array<int, 8> ANTI_DIR_IDX;
    static Array<double, 8> DIR_RATE;

    static int GetDashDirIdx(const AngleDeg & dir) {
    	for (int i = 0; i < 8; ++i) {
    		if (GetAngleDegDiffer(dir, DASH_DIR[i]) < FLOAT_EPS) return i;
    	}
    	Assert(0);
    	return -1;
    }

    /** �����ķ�ʽ�ܵ�Ŀ���
     * Run to the destination point with the fastest method.
     * \param agent the agent itself.
     * \param pos the destination point.
     * \param buffer point buffer, means the real destinantion is a cycle with the certer 
     * 		of pos and radius of buffer.
     * \param power the intend power to use.
     * \param can_inverse true means can run in the inverse direction of the agent's body.
     * \param turn_first true means that the agent should turn first to adjust the direction.
     * \return true if the action excuted successfully.
     */
    bool GoToPoint(Agent & agent, Vector pos, double buffer  = 0.5, double power = 100.0, bool can_inverse = true, bool turn_first = false);

    /** �����ķ�ʽ�ܵ�Ŀ���
     * Run to the destination point with the fastest method.
     * \param agent the agent itself.
     * \param act an atomic action caculated to go to pos for this cycle.
     * \param pos the destination point.
     * \param buffer point buffer, means the real destinantion is a cycle with the certer 
     * 		of pos and radius of buffer.
     * \param power the intend power to use.
     * \param can_inverse true means can run in the inverse direction of the agent's body.
     * \param turn_first true means that the agent should turn first to adjust the direction.
     */
    void GoToPoint(Agent & agent, AtomicAction & act, Vector pos, double buffer  = 0.5, double power = 100.0, bool can_inverse = true, bool turn_first = false);

    /** ��ȷ�������ƣ�ָ�����ܺ����ܣ����ܵ�Ŀ���
     * Run to the destination point with a certain posture, forward or backword.
     * \param agent the agent itself.
     * \param act an atomic action caculated to go to pos for this cycle.
     * \param pos the destination point.
     * \param buffer point buffer, means the real destinantion is a cycle with the center
     * 		of pos and radius of buffer.
     * \param power the intend power to use.
     * \param inverse true means run backward.
     * \param turn_first true means that the agent should turn first to adjust the direction.
     */
    void GoToPointWithCertainPosture(Agent & agent, AtomicAction & act, Vector pos, double buffer  = 0.5, double power = 100.0, const bool inverse = false, bool turn_first = false);

    /**
     * player �ܵ� target ���������С������
     * This function returns the minimum cycles for a player to go to a target position.
     * @param player the player to caculate.
     * @param target the target position to go to.
     * @param can_inverse true means consider running backwards.
     * @param buffer
     * @return an integer to show the minimum cycles caculated.
     */
    int CycleNeedToPoint(const PlayerState & player, Vector target, bool can_inverse = true, double *buf = 0);

     /**
     * player �ܵ� target ���������С������, ����ʵ������
     * This function returns the minimum cycles for a player to go to a target position.
     * @param player the player to caculate.
     * @param target the target position to go to.
     * @param can_inverse true means consider running backwards.
     * @return a double to show the minimum cycles caculated.
     */
    double RealCycleNeedToPoint(const PlayerState & player, Vector target, bool can_inverse = true)
    {
    	double buf;
    	int cyc = CycleNeedToPoint(player, target, can_inverse, & buf);
    	return cyc - buf;
    }

    /**
     * player ��ȷ�������ƣ�ָ�����ܺ����ܣ����ܵ� target ����Ҫ��������
     * This function returns the minimum cycles for a player to go to a target position with 
     * a certain posture, forward or backward.
     * @param player the player to caculate.
     * @param target the target position to go to.
     * @param inverse true means running backwards.
     * @param buffer 
     * @return an integer to show the minimum cycles caculated.
     */
    int CycleNeedToPointWithCertainPosture(const PlayerState & player, Vector target, const bool inverse, double *buf = 0);

    /**
     * player ��ȷ�������ƣ�ָ�����ܺ����ܣ����ܵ� target ����Ҫ��������, ����ʵ������
     * This function returns the minimum cycles for a player to go to a target position with 
     * a certain posture, forward or backward.
     * @param player the player to caculate.
     * @param target the target position to go to.
     * @param inverse true means running backwards.
     * @return an integer to show the minimum cycles caculated.
     */
    double RealCycleNeedToPointWithCertainPosture(const PlayerState & player, Vector target, const bool inverse)
    {
    	double buf;
    	int cyc = CycleNeedToPointWithCertainPosture(player, target, inverse, & buf);
    	return cyc - buf;
    }

    /**
     * ������ͬһdash_power����ĳ����Ҫ��ʱ��
     * Caculate cycles needed to a target position with a certain dash power.
     * @param player the player to caculate.
     * @param target the target position to go to.
     * @param dash_power the power for dash to predict.
     * @return an integer to show the cycles caculated.
     */
    int CyclePredictedToPoint(const PlayerState& player , Vector target , double dash_power);


    /**
     * ��Ҫ��Ϊ�˵���power�ã�Ϊ�˱���power������ܹ���
     * Ŀ��㡣ԭ����Ϊ�˸��߲�Э����powerֻ���ܼ�С����
     * ������Ϊ�˲���Ҫ��dash��powerҲ�ɿ���Ϊ�㡣Ҳ��
     * dash��ִ��
     * power ���������������
     * This funcition is mainly used to adjust ( usually decrease ) the dash power 
     * to avoid run over the target position by using a big power. The final power 
     * could be 0 to avoid an unnecessary dash.
     * The sign of power is given outside the function.
     * @param player the player to caculate.
     * @param target the target position to go to.
     * @param buffer
     * @param power orignal power given.
     * @return power adjusted.
     */
    double AdjustPowerForDash(const PlayerState & player, Vector target, double buffer, double power);

    /**
     * ����ת����ֱ��dash
     * Planing to turn or to dash.
     * @param agent the agent itself.
     * @param act an atomic action to return the caculated decision.
     * @param target the target position to go to.
     * @param buffer
     * @param power the intend power for dash.
     * @param inverse true means running backwards.
     * @param turn_first true means turn first manually.
     */
    void TurnDashPlaning(Agent & agent, AtomicAction & act, Vector target, double buffer, double power, bool inverse, bool turn_first = false);

    /**
     * Caculate the get ball cycles and actions.
     * @param agent the agent itself.
     * @param act the atomic action to execute this cycle to get the ball.
     * @param int_cycle the beginning cycle to do the action, while -1 means now.
     * @param can_inverse true means allow runnning backwards to get the ball.
     * @param turn_first true means the agent turn first to get the ball.
     * @return a double to show the cycles needed and if it less than 0, that is to say impossible.
     */
    double GetBall(Agent & agent, AtomicAction & act, int int_cycle = -1, bool can_inverse = true, bool turn_first = false);

    /**
     * Caculate the get ball action and execute it.
     * @param agent the agent itself.
     * @param int_cycle the beginning cycle to do the action, while -1 means now.
     * @param can_inverse true means allow runnning backwards to get the ball.
     * @param turn_first true means the agent turn first to get the ball.
     * @return true means the action executed successfully.
     */
    bool GetBall(Agent & agent, int int_cycle = -1, bool can_inverse = true, bool turn_first = false);

    /*
     * This function is used to correct the target position when near the ball.
     * @param player the player to consider.
     * @param target the target position to go to.
     * @param fix the distance error to fix.
     * @return position corrected.
     */
    Vector CorrectTurnForDash(const PlayerState & player, const Vector & target, double fix = 0.0);

// �����ṩ�й�����agent������Ϊ�Ľӿڣ�ֱ�ӷ���AtomicAction�����ⲿ����
// The following funtions are some interfaces for agent to do some basic behaviors 
// without ball. The return value are all AtomicAction. 
public:
    /**
     * ������ת���ض�����
     * Turn body to a certain direction.
     * @param agent the agent itself.
     * @param ang the angle to turn to.
     * @return an atomic action to turn the body.
     */
    AtomicAction GetTurnBodyToAngleAction(const Agent & agent, AngleDeg ang);

    /*! This method determines the optimal dash power to mantain an optimal speed
	When the current speed is too high and the distance is very small, a
	negative dash is performed. Otherwise the difference with the maximal speed
	is determined and the dash power rate is set to compensate for this
	difference.
	\param posRelTo relative point to which we want to dash
	\param angBody body angle of the agent
	\param vel current velocity of the agent
	\param dEffort current effort of the player
	\param iCycles desired number of cycles to reach this point
	\return dash power that should be sent with dash command */
    bool GetPowerForForwardDash(const Agent &agent, double* dash_power, Vector posRelTo, double angBody, double dEffort, int iCycles );

public:
	static double GETBALL_BUFFER; //��������ʹ�õ��ж��Ƿ���ߵ�buf����worldstate��Ĵ�
};

#endif