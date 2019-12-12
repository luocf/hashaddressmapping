#include <Command/HashAddressMappingCmd.hpp>

#include <iostream>
#include <iterator>
#include <sstream>
namespace micro_service {
/* =========================================== */
/* === variables initialize =========== */
/* =========================================== */
    const std::vector <HashAddressMappingCmd::CommandInfo> HashAddressMappingCmd::gCommandInfoList{
            {"h", "help",   HashAddressMappingCmd::Help,           "Print help usages."},
            {"g", "get",   HashAddressMappingCmd::GetAddress,    "Get address."},
    };
    
    inline std::string &ltrim(std::string &str) {
        std::string::iterator p = std::find_if(str.begin(), str.end(), std::not1(std::ptr_fun<int, int>(isspace)));
        str.erase(str.begin(), p);
        return str;
    }
/* =========================================== */
/* === function implement ============= */
/* =========================================== */
    int HashAddressMappingCmd::Do(void *context,
                         const std::string &cmd_msg,
                         std::string &errMsg) {
        std::string trim_msg = cmd_msg;
        trim_msg = ltrim(trim_msg);
        if (trim_msg.find('/') != 0) {
            errMsg = "not command";
            return -10000;
        }

        const std::string &cmdLine = trim_msg.substr(1);
        auto wsfront = std::find_if_not(cmdLine.begin(), cmdLine.end(),
                                        [](int c) { return std::isspace(c); });
        auto wsback = std::find_if_not(cmdLine.rbegin(), cmdLine.rend(),
                                       [](int c) { return std::isspace(c); }).base();
        auto trimCmdLine = (wsback <= wsfront ? std::string() : std::string(wsfront, wsback));

        std::istringstream iss(trimCmdLine);
        std::vector <std::string> args{std::istream_iterator < std::string > {iss},
                                       std::istream_iterator < std::string > {}};
        if (args.size() <= 0) {
            return 0;
        }
        const auto &cmd = args[0];

        for (const auto &cmdInfo : gCommandInfoList) {
            if (cmd.compare(0, 1, cmdInfo.mCmd) != 0
                && cmd != cmdInfo.mLongCmd) {
                continue;
            }

            int ret = cmdInfo.mFunc(context, args, errMsg);
            return ret;
        }

        errMsg = "Unknown command: " + cmd;
        return -10000;
    }

/* =========================================== */
/* === class public function implement  ====== */
/* =========================================== */

/* =========================================== */
/* === class protected function implement  === */
/* =========================================== */


/* =========================================== */
/* === class private function implement  ===== */
/* =========================================== */

    int HashAddressMappingCmd::Help(void *context,
                           const std::vector <std::string> &args,
                           std::string &errMsg) {
        std::cout << "Usage:" << std::endl;
        std::string msg = "";

        for (const auto &cmdInfo : gCommandInfoList) {
            msg += "" + cmdInfo.mCmd + " | " + cmdInfo.mLongCmd + " : " + cmdInfo.mUsage + "\n";
        }
        auto carrier_robot = reinterpret_cast<HashAddressMappingService *>(context);
        carrier_robot->helpCmd(args, msg);
        return 0;
    }
    
    int HashAddressMappingCmd::GetAddress(void *context,
                                  const std::vector <std::string> &args,
                                  std::string &errMsg) {
        auto carrier_robot = reinterpret_cast<HashAddressMappingService *>(context);
        carrier_robot->replyAddressCmd(args);
        return 0;
    }
}