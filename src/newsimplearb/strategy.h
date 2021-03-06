#ifndef SRC_NEWSIMPLEARB_STRATEGY_H_
#define SRC_NEWSIMPLEARB_STRATEGY_H_

#include <market_snapshot.h>
#include <strategy_status.h>
#include <timecontroller.h>
#include <Contractor.h>
#include <order.h>
#include <sender.h>
#include <caler.h>
#include <exchange_info.h>
#include <order_status.h>
#include <common_tools.h>
#include <base_strategy.h>
#include <libconfig.h++>
#include <unordered_map>

#include <cmath>
#include <vector>
#include <string>
#include <iostream>
#include <memory>

class Strategy : public BaseStrategy {
 public:
  explicit Strategy(const libconfig::Setting & param_setting, const libconfig::Setting & ticker_setting, const TimeController& tc, std::unordered_map<std::string, std::vector<BaseStrategy*> >*ticker_strat_map, Contractor& ct, Sender* sender, Sender* ordersender, const std::string & mode = "real", bool no_close_today = false);
  virtual ~Strategy();

  void Start() override;
  void Stop() override;

  void Clear() override;
  void HandleCommand(const Command& shot) override;
  void UpdateTicker() override;
 private:
  void ClearPositionRecord();
  void DoOperationAfterUpdateData(const MarketSnapshot& shot) override;
  void DoOperationAfterUpdatePos(Order* o, const ExchangeInfo& info) override;
  void DoOperationAfterFilled(Order* o, const ExchangeInfo& info) override;
  void DoOperationAfterCancelled(Order* o) override;
  void ModerateOrders(const std::string & ticker) override;
  void Init() override;
  bool Ready() override;
  void Pause() override;
  void Resume() override;
  void Run() override;
  void Train() override;
  void Flatting() override;

  void UpdateBuildPosTime();

  double OrderPrice(const std::string & ticker, OrderSide::Enum side, bool control_price) override;

  OrderSide::Enum OpenLogicSide();
  bool OpenLogic();
  void CloseLogic();

  void Open(OrderSide::Enum side);
  bool Close(bool force_flat = false);

  void CalParams();
  bool HitMean();

  double GetPairMid();

  void ForceFlat() override;

  bool Spread_Good();

  bool IsAlign();

  void UpdateBound(OrderSide::Enum side);
  void StopLossLogic();
  void HandleTestOrder(Order *o);

  char order_ref[MAX_ORDERREF_SIZE];
  std::string main_ticker;
  std::string hedge_ticker;
  int max_pos;
  double min_price_move;

  TimeController this_tc;
  std::unordered_map<std::string, std::vector<BaseStrategy*> >*tsm;
  int cancel_limit;
  std::unordered_map<std::string, double> mid_map;
  double up_diff;
  double down_diff;
  double range_width;
  double mean;
  std::vector<double> map_vector;
  int current_pos;
  double min_profit;
  unsigned int min_train_sample;
  double min_range;
  double increment;
  std::string mode;
  double spread_threshold;
  int closed_size;
  double last_valid_mid;
  double stop_loss_up_line;
  double stop_loss_down_line;
  int max_loss_times;
  double stop_loss_times;
  double stop_loss_margin;
  double open_fee_rate;
  double close_today_fee_rate;
  double close_fee_rate;
  double deposit_rate;
  double round_fee_cost;
  int max_close_try;
  double current_spread;
  CALER * caler;
  bool is_started;
  Sender* data_sender;
  bool no_close_today;
  int open_count;
  int close_count;
  int main_record;
  int hedge_record;
  MarketSnapshot main_last;
  MarketSnapshot hedge_last;
  double total_slip_loss;
};

#endif  // SRC_NEWSIMPLEARB_STRATEGY_H_
