#include "Path.h"
#include "Trajectory.h"
#include "csv2.hpp"

#include <chrono>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <vector>

#include <spdlog/spdlog.h>

#include <spdlog/fmt/ostr.h>

#include <Eigen/Dense>

template <typename T>
struct fmt::formatter<T, std::enable_if_t<fmt::v9::detail::is_streamable<T, char>::value, char>>
    : fmt::ostream_formatter
{
};

int main(int argn, char **argv)
{
    if (argn < 2)
    {
        SPDLOG_ERROR("filename is needed!");
        return -1;
    }

    csv2::Reader<csv2::delimiter<' '>, csv2::quote_character<'"'>, csv2::first_row_is_header<false>,
                 csv2::trim_policy::trim_whitespace>
        csv;

    std::list<Eigen::VectorXd> waypoints;
    Eigen::VectorXd max_accs = Eigen::VectorXd::Ones(5) * 100000.;
    Eigen::VectorXd max_vels;

    if (csv.mmap(argv[1]))
    {
        for (const auto &row : csv)
        {
            Eigen::VectorXd waypoint(5);
            int i = 0;
            for (const auto &cell : row)
            {
                std::string v;
                cell.read_value(v);
                if (i == 5)
                {
                    double speed = std::stod(v);
                    max_vels = Eigen::VectorXd::Ones(5) * speed;
                    break;
                }
                waypoint[i++] = std::stod(v);
            }
            waypoints.push_back(waypoint);
        }
    }
    for (auto &waypoint : waypoints)
    {
        SPDLOG_INFO("waypoint: {}", waypoint.transpose());
    }

    Path path(waypoints, 0.1);
    Trajectory traj(path, max_vels, max_accs, 0.01);
    SPDLOG_INFO("traj valid: {}, duration: {}", traj.isValid(), traj.getDuration());
    double t = 0;
    while (t < traj.getDuration())
    {
        SPDLOG_INFO("t: {}, pos: {}", t, traj.getPosition(t));
        t += 0.01;
    }
    return 0;
}
