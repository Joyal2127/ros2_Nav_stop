#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "nav2_msgs/action/navigate_to_pose.hpp"

class StopRobot : public rclcpp::Node {
public:
    using NavigateToPose = nav2_msgs::action::NavigateToPose;
    using GoalHandleNavigateToPose = rclcpp_action::ClientGoalHandle<NavigateToPose>;

    StopRobot() : Node("stop_robot") {
        client_ptr_ = rclcpp_action::create_client<NavigateToPose>(this, "navigate_to_pose");

        if (!client_ptr_->wait_for_action_server(std::chrono::seconds(10))) {
            RCLCPP_ERROR(this->get_logger(), "Action server not available after waiting");
            return;
        }

        auto goal_handle = client_ptr_->async_cancel_all_goals();
        RCLCPP_INFO(this->get_logger(), "Cancel request sent");
    }

private:
    rclcpp_action::Client<NavigateToPose>::SharedPtr client_ptr_;
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<StopRobot>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
