//
// Created by bernardoct on 6/26/17.
//

#include "WwtpDischargeRule.h"

WwtpDischargeRule::WwtpDischargeRule(const vector<vector<double>> *year_series_fraction_discharge,
                                     const vector<int> *discharge_to_source_ids) : year_series_fraction_discharge(
        year_series_fraction_discharge), discharge_to_source_ids(discharge_to_source_ids) {

    __throw_invalid_argument("Number of wwtp discharge time series must be the same as number of sources ids.");
}

/**
 * Returns the fraction of week's demand to be discharged as effluent.
 * @param water_source_id ID of source receiving the effluent.
 * @return fraction of demand to be discharged.
 */
double WwtpDischargeRule::get_dependent_variable(int water_source_id, int week) {
    return (*year_series_fraction_discharge)[water_source_id][week];
}

double WwtpDischargeRule::get_dependent_variable(double x, int week) {
    __throw_invalid_argument("WWTP discharge rules need a water source ID (int) and week number (int) to return "
                                     "the fraction of demand discharged to that source.");
}

double WwtpDischargeRule::get_dependent_variable(double x) {
    __throw_invalid_argument("WWTP discharge rules need a water source ID (int) and week number (int) to return "
                                     "the fraction of demand discharged to that source.");
}

double WwtpDischargeRule::get_dependent_variable(int x) {
    __throw_invalid_argument("WWTP discharge rules need a water source ID (int) and week number (int) to return "
                                     "the fraction of demand discharged to that source.");
}

WwtpDischargeRule::WwtpDischargeRule(WwtpDischargeRule &wwtp_discharge_rule) :
        discharge_to_source_ids(wwtp_discharge_rule.discharge_to_source_ids),
        year_series_fraction_discharge(wwtp_discharge_rule.year_series_fraction_discharge) {}

WwtpDischargeRule &WwtpDischargeRule::operator=(const WwtpDischargeRule &wwtp_discharge_rule) {
    discharge_to_source_ids = wwtp_discharge_rule.discharge_to_source_ids;
    year_series_fraction_discharge = wwtp_discharge_rule.year_series_fraction_discharge;
    return *this;
}
