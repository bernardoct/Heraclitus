//
// Created by bernardo on 1/12/17.
//

#include <iostream>
#include <cmath>
#include "ContinuityModel.h"


ContinuityModel::ContinuityModel(const vector<WaterSource *> &water_sources, const vector<Utility *> &utilities,
                                 const Graph &water_sources_graph,
                                 const vector<vector<int>> &water_sources_to_utilities) : continuity_water_sources(
        water_sources),
                                                                                          continuity_utilities(
                                                                                                  utilities),
                                                                                          water_sources_graph(
                                                                                                  water_sources_graph),
                                                                                          water_sources_to_utilities(
                                                                                                  water_sources_to_utilities) {
    /// Connect water sources to utilities.
    for (int i = 0; i < utilities.size(); ++i) {
        for (int j = 0; j < water_sources_to_utilities[i].size(); ++j) {
            this->continuity_utilities[i]->addWaterSource(
                    water_sources[water_sources_to_utilities[i][j]]);
        }
    }

    utilities_to_water_sources.assign(water_sources.size(), vector<int>(0));
    for (int i = 0; i < utilities.size(); ++i) {
        for (const int &j : water_sources_to_utilities[i]) {
            utilities_to_water_sources[j].push_back(i);
        }
    }

    /// Get topological order so that mass balance is ran from up to downstream.
    reservoir_continuity_order = water_sources_graph.getTopological_order();

    water_sources_draws = vector<double>(water_sources.size(), 0.);

    /// the variables below are to make the storage-ROF table calculation faster.
    for (int ws = 0; ws < water_sources.size(); ++ws) {
        if (water_sources[ws]->isOnline())
            water_sources_capacities.push_back(water_sources[ws]->capacity);
        else
            water_sources_capacities.push_back((double &&) NONE);
    }

    for (Utility *u : continuity_utilities)
        utilities_capacities.push_back(u->getTotal_storage_capacity());

    for (vector<int> ds : water_sources_graph.getDownSources())
        if (ds.empty())
            downstream_sources.push_back(NON_INITIALIZED);
        else
            downstream_sources.push_back(ds[0]);

    sources_topological_order = water_sources_graph.getTopological_order();
}

ContinuityModel::~ContinuityModel() {

}

ContinuityModel::ContinuityModel(ContinuityModel &continuity_model) {}

/**
 * Calculates continuity for one week time step for streamflows of id_rof years before current week.
 * @param week current week.
 * @param rof_realization rof realization id (between 0 and 49 inclusive).
 */
void ContinuityModel::continuityStep(int week, int rof_realization) {
    double demands[continuity_water_sources.size()] = {};
    double upstream_spillage[continuity_water_sources.size()] = {};

    /**
     * Split weekly demands among each reservoir for each utility. For each water source:
     * (1) sums the demands of each drawing utility to come up with the total unrestricted_demand for
     * that week for that water source, and (2) sums the flow contributions of upstream
     * reservoirs.
     */
    for (Utility *u : continuity_utilities) {
        u->splitDemands(week, demands);
    }

    /**
     * For all water sources, performs mass balance to update the available volume. The week here is
     * shifted back according to the rof year realization (0 to 49, inclusive) so that the right flows are
     * gotten from source catchments for each rof year realization. If this is not an rof calculation but an actual
     * simulation instead, rof_realization will be equal to -1 (see header file) so that there is no week shift.
     */
    for (int &i : reservoir_continuity_order) {
        for (int &ws : water_sources_graph.getUpstream_sources(i))
            upstream_spillage[i] += continuity_water_sources[ws]->getTotal_outflow();

        continuity_water_sources[i]->continuityWaterSource(
                week - (int) std::round((rof_realization + 1) * WEEKS_IN_YEAR),
                upstream_spillage[i], demands[i]);
    }

    /// updates combined storage for utilities.
    for (Utility *u : continuity_utilities) {
        u->updateTotalStoredVolume();
    }
//    water_sources_draws = vector<double<();
}

const vector<Utility *> &ContinuityModel::getUtilities() const {
    return continuity_utilities;
}




