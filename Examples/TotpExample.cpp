#include "csv2.hpp"

#include <Totp/Trajectory.h>

#include <Eigen/Dense>

#include <spdlog/spdlog.h>

#include <fmt/os.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/stopwatch.h>

#include <cxxopts.hpp>

#include <chrono>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <vector>

template <typename T>
struct fmt::formatter<T, std::enable_if_t<fmt::v9::detail::is_streamable<T, char>::value, char>>
    : fmt::ostream_formatter
{
};

int main(int argc, char **argv)
{
    cxxopts::Options options("TotpExample", "Use totp algorithm to compute a trajectory.");
    // clang-format off
    options.add_options()
    ("i,input", "input waypoints file", cxxopts::value<std::string>())
    ("o,output", "output sampled waypoints file", cxxopts::value<std::string>()->default_value("./out_waypoints.txt"))
    ("t,interval", "sample time interval", cxxopts::value<double>()->default_value("0.002"))
    ("h,help", "Print usage")
    ;
    // clang-format on
    auto result = options.parse(argc, argv);
    if (result.count("help"))
    {
        fmt::print("{}\n", options.help());
        exit(0);
    }

    csv2::Reader<csv2::delimiter<' '>, csv2::quote_character<'"'>, csv2::first_row_is_header<false>,
                 csv2::trim_policy::trim_whitespace>
        csv;

    std::list<Eigen::VectorXd> waypoints;
    Eigen::VectorXd max_accs = Eigen::VectorXd::Ones(5) * 100000.;
    Eigen::VectorXd max_vels;

    if (csv.mmap(result["input"].as<std::string>()))
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
        spdlog::debug("waypoint: {}", waypoint.transpose());
    }

    spdlog::info("start to compute trajectory...");
    spdlog::stopwatch sw;
    Path path(waypoints, 0.1);
    Trajectory traj(path, max_vels, max_accs, 0.01);
    spdlog::info("finish computing trajectory, time elapsed: {} seconds", sw);
    spdlog::info("traj valid: {}, duration: {}", traj.isValid(), traj.getDuration());

    auto out = fmt::output_file(result["output"].as<std::string>());
    double dt = result["interval"].as<double>();

    double t = 0;
    out.print("{:^8s} {:^8s} {:^8s} {:^8s} {:^8s} {:^8s}\n", "时间", "x", "y", "z", "c", "a");
    while (t < traj.getDuration())
    {
        auto pos = traj.getPosition(t);
        out.print("{:^8.3f} {:^8.3f} {:^8.3f} {:^8.3f} {:^8.3f} {:^8.3f}\n", t, pos[0], pos[1], pos[2], pos[3], pos[4]);
        t += dt;
    }
    return 0;
}
