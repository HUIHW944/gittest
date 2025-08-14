#include <stdint.h>
#include <stdbool.h>

typedef enum {
    HcuStctrlReq_Disable = 0,
    HcuStctrlReq_Enable
} DcdcStaCtrlWrap_TEHcuStCtrlReq;

typedef enum {
    STATE_INIT = 0,      // 初始化
    STATE_STANDBY,       // 待机（使能但未工作）
    STATE_ACTIVE,        // 工作（正常输出）
    STATE_ERROR,         // 故障
    STATE_COUNT          // 状态总数（辅助用）
} DcdcState;

// 3. 定义状态机结构体（保存当前状态和上下文）
typedef struct {
    DcdcState current_state;          // 当前状态
    bool is_overvoltage;              // 过压标志（示例故障条件）
    bool is_overcurrent;              // 过流标志（示例故障条件）
    // 其他上下文：如电压、电流、配置参数等
} DcdcFsm;

// 4. 状态机初始化函数
void dcdc_fsm_init(DcdcFsm *fsm) {
    fsm->current_state = STATE_INIT;
    fsm->is_overvoltage = false;
    fsm->is_overcurrent = false;
    // 初始化硬件（如禁用 HCU）
    DcdcStaCtrlWrap_TEHcuStCtrlReq req = HcuStctrlReq_Disable;
    // ... 调用硬件驱动设置 req ...
}

// 5. 状态处理函数（每个状态的逻辑）
static void handle_init_state(DcdcFsm *fsm, DcdcStaCtrlWrap_TEHcuStCtrlReq req) {
    if (req == HcuStctrlReq_Enable) {
        // 使能请求：进入待机状态
        fsm->current_state = STATE_STANDBY;
        // 调用硬件驱动使能 HCU
    } else {
        // 保持初始化状态（或处理禁用逻辑）
    }
}

static void handle_standby_state(DcdcFsm *fsm, DcdcStaCtrlWrap_TEHcuStCtrlReq req) {
    if (req == HcuStctrlReq_Disable) {
        // 禁用请求：回到初始化状态
        fsm->current_state = STATE_INIT;
        // 调用硬件驱动禁用 HCU
    } else {
        // 使能状态下，检查是否需要进入工作状态
        // 假设检测到“启动信号”（示例条件）
        bool start_signal = true; // 实际应从硬件读取
        if (start_signal) {
            fsm->current_state = STATE_ACTIVE;
        }
    }
}

static void handle_active_state(DcdcFsm *fsm, DcdcStaCtrlWrap_TEHcuStCtrlReq req) {
    if (req == HcuStctrlReq_Disable) {
        // 禁用请求：回到初始化状态
        fsm->current_state = STATE_INIT;
        // 调用硬件驱动禁用 HCU
    } else {
        // 检查故障条件
        if (fsm->is_overvoltage || fsm->is_overcurrent) {
            // 故障：进入错误状态
            fsm->current_state = STATE_ERROR;
        }
        // 否则继续工作
    }
}

static void handle_error_state(DcdcFsm *fsm, DcdcStaCtrlWrap_TEHcuStCtrlReq req) {
    // 故障状态：优先处理错误
    if (req == HcuStctrlReq_Disable) {
        // 禁用请求：回到初始化状态
        fsm->current_state = STATE_INIT;
        // 调用硬件驱动禁用 HCU
    } else {
        // 尝试恢复（示例逻辑：清除故障标志）
        fsm->is_overvoltage = false;
        fsm->is_overcurrent = false;
        // 回到待机状态重试
        fsm->current_state = STATE_STANDBY;
    }
}

// 6. 状态机主循环（定期调用）
void dcdc_fsm_run(DcdcFsm *fsm, DcdcStaCtrlWrap_TEHcuStCtrlReq req) {
    // 先检查故障（优先级最高）
    if (fsm->is_overvoltage || fsm->is_overcurrent) {
        fsm->current_state = STATE_ERROR;
    }

    // 根据当前状态调度处理函数
    switch (fsm->current_state) {
        case STATE_INIT:
            handle_init_state(fsm, req);
            break;
        case STATE_STANDBY:
            handle_standby_state(fsm, req);
            break;
        case STATE_ACTIVE:
            handle_active_state(fsm, req);
            break;
        case STATE_ERROR:
            handle_error_state(fsm, req);
            break;
        default:
            // 无效状态处理
            break;
    }
}

// 7. 示例：触发故障（外部调用，如硬件中断）
void dcdc_trigger_overvoltage(DcdcFsm *fsm) {
    fsm->is_overvoltage = true;
}

// 8. 测试代码
int main() {
    DcdcFsm fsm;
    dcdc_fsm_init(&fsm);

    // 模拟状态切换
    DcdcStaCtrlWrap_TEHcuStCtrlReq req = HcuStctrlReq_Enable;
    dcdc_fsm_run(&fsm, req); // 从 INIT -> STANDBY

    req = HcuStctrlReq_Enable;
    dcdc_fsm_run(&fsm, req); // STANDBY -> ACTIVE（假设启动信号有效）

    dcdc_trigger_overvoltage(&fsm);
    dcdc_fsm_run(&fsm, req); // ACTIVE -> ERROR（检测到过压）

    return 0;
}
