//
// Created by bernardo on 1/13/17.
//

#ifndef TRIANGLEMODEL_UTILITY_H
#define TRIANGLEMODEL_UTILITY_H


#include <map>
#include "WaterSources/Reservoir.h"
#include "../Utils/Constants.h"
#include "../Controls/WwtpDischargeRule.h"
//#include "../Utils/Matrix3D.h"

//#define DOLLARS_PER_GAL_TO_DOLLARS_PER_MMGAL 1e-6

class Utility {
private:
    float *demand_series = nullptr;
    float *weekly_average_volumetric_price;
    vector<int> priority_draw_water_source;
    vector<int> non_priority_draw_water_source;
    vector<float> weekly_peaking_factor;
    vector<float> infra_construction_triggers;
    float short_term_risk_of_failure = 0;
    float long_term_risk_of_failure = 0;
    float total_storage_capacity = 0;
    float total_stored_volume = 0;
    float total_treatment_capacity = 0;
    float waste_water_discharge = 0;
    float gross_revenue = 0;
    float unfulfilled_demand = 0;
    bool used_for_realization = true;
    vector<WaterSource *> water_sources;
    vector<vector<float>> *demands_all_realizations;
    WwtpDischargeRule wwtp_discharge_rule;

    /// Drought mitigation
    float fund_contribution = 0;
    float demand_multiplier = 1;
    float demand_offset = 0;
    float restricted_price = NON_INITIALIZED;
    float offset_rate_per_volume = 0;
    float contingency_fund = 0;
    float drought_mitigation_cost = 0;
    float insurance_payout = 0;
    float insurance_purchase = 0;
    float restricted_demand = 0;
    float unrestricted_demand = 0;
    int n_sources = 0;

    /// Infrastructure cost
    float current_debt_payment = 0;
    vector<vector<float>> debt_payment_streams;
    float infra_net_present_cost = 0;

    /// Infrastructure construction
    vector<int> under_construction_id;
    vector<int> infra_built_last_week;
    vector<int> rof_infra_construction_order;
    vector<int> demand_infra_construction_order;
    vector<int> construction_end_date;
    vector<bool> under_construction;

public:
    const int id;
    const int number_of_week_demands;
    const char *name;
    const float percent_contingency_fund_contribution;
    const float infra_discount_rate;
    const float demand_buffer;
    const vector<vector<int>> *infra_if_built_remove;

    Utility(
            const char *name, int id,
            vector<vector<float>> *demands_all_realizations,
            int number_of_week_demands,
            const float percent_contingency_fund_contribution,
            const vector<vector<float>> *typesMonthlyDemandFraction,
            const vector<vector<float>> *typesMonthlyWaterPrice,
            WwtpDischargeRule wwtp_discharge_rule,
            float demand_buffer);

    Utility(const char *name, int id,
                vector<vector<float>> *demands_all_realizations,
                int number_of_week_demands,
                const float percent_contingency_fund_contribution,
                const vector<vector<float>> *typesMonthlyDemandFraction,
                const vector<vector<float>> *typesMonthlyWaterPrice,
                WwtpDischargeRule wwtp_discharge_rule,
                float demand_buffer,
                const vector<int> &rof_infra_construction_order,
                const vector<int> &demand_infra_construction_order,
                const vector<float> &infra_construction_triggers,
                float infra_discount_rate,
                const vector<vector<int>> *infra_if_built_remove);

    Utility(const char *name, int id,
                vector<vector<float>> *demands_all_realizations,
                int number_of_week_demands,
                const float percent_contingency_fund_contribution,
                const vector<vector<float>> *typesMonthlyDemandFraction,
                const vector<vector<float>> *typesMonthlyWaterPrice,
                WwtpDischargeRule wwtp_discharge_rule,
                float demand_buffer,
                const vector<int> &rof_infra_construction_order,
                const vector<int> &demand_infra_construction_order,
                const vector<float> &infra_construction_triggers,
                float infra_discount_rate);

    Utility(Utility &utility);

    ~Utility();

    Utility &operator=(const Utility &utility);

    bool operator<(const Utility* other);

    bool operator>(const Utility* other);

    static bool compById(Utility *a, Utility *b);

    void setRisk_of_failure(float risk_of_failure);

    void updateTotalStoredVolume();

    void calculateWastewater_releases(int week, float *discharges);

    void addWaterSource(WaterSource *water_source);

    void splitDemands(
            int week, vector<vector<float>> &demands, bool
    apply_demand_buffer = false);

    void calculateWeeklyAverageWaterPrices(
            const vector<vector<float>> *typesMonthlyDemandFraction,
            const vector<vector<float>> *typesMonthlyWaterPrice);

    float waterPrice(int week);

    void addWaterSourceToOnlineLists(int source_id);

    void
    forceInfrastructureConstruction(int week, vector<int> new_infra_triggered);

    void checkErrorsAddWaterSourceOnline(WaterSource *water_source);

    int infrastructureConstructionHandler(float long_term_rof, int week);

    void reservoirExpansionConstructionHandler(unsigned int source_id);

    void waterTreatmentPlantConstructionHandler(unsigned int source_id);

    void priceCalculationErrorChecking(
            const vector<vector<float>> *typesMonthlyDemandFraction,
            const vector<vector<float>> *typesMonthlyWaterPrice);

    float getTotal_storage_capacity() const;

    float getRisk_of_failure() const;

    float getStorageToCapacityRatio() const;

    float getGrossRevenue() const;

    void setDemand_multiplier(float demand_multiplier);

    void setDemand_offset(float demand_offset, float offset_rate_per_volume);

    float getTotal_treatment_capacity() const;

    void updateContingencyFund(
            float unrestricted_demand, float demand_multiplier,
            float demand_offset, float unfulfilled_demand, int week);

    void beginConstruction(int week, int infra_id);

    void setWaterSourceOnline(unsigned int source_id);

    float updateCurrent_debt_payment(int week);

    float getContingency_fund() const;

    float getUnrestrictedDemand() const;

    float getRestrictedDemand() const;

    void setRestricted_price(float restricted_price);

    float getDemand_multiplier() const;

    float getUnrestrictedDemand(int week) const;

    float getInfrastructure_net_present_cost() const;

    float getCurrent_debt_payment() const;

    float getCurrent_contingency_fund_contribution() const;

    float getDrought_mitigation_cost() const;

    void addInsurancePayout(float payout_value);

    void clearWaterSources();

    void purchaseInsurance(float insurance_price);

    float getInsurance_payout() const;

    float getInsurance_purchase() const;

    const vector<int> &getRof_infrastructure_construction_order() const;

    void setRealization(unsigned long r);

    const vector<int> getInfrastructure_built() const;

    void setNoFinaicalCalculations();

    float getLong_term_risk_of_failure() const;

    const vector<int> &getDemand_infra_construction_order() const;

    vector<float> calculateWeeklyPeakingFactor(vector<float> *demands);

    void sourceRelocationConstructionHandler(unsigned int source_id);

    const vector<WaterSource *> &getWater_sources() const;

    void removeRelatedSourcesFromQueue(int next_construction);

    float getWaste_water_discharge() const;

    vector<float>
    rearrangeInfraRofVector(const vector<float> &infra_construction_triggers,
                            const vector<int> &rof_infra_construction_order,
                            const vector<int> &demand_infra_construction_order);

    float getTotal_stored_volume() const;

    void resetTotal_storage_capacity();

    float getUnfulfilled_demand() const;
};


#endif //TRIANGLEMODEL_UTILITY_H
