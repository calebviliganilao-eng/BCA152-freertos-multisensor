#include <unity.h>

void setUp(void) {}
void tearDown(void) {}

enum DisplayMode {
    MODE_TEMPERATURE,
    MODE_HUMIDITY,
    MODE_LIGHT,
    MODE_MOTION
};

enum SystemState { INACTIVE, ACTIVE };

int evaluateTemperature(float temp) {
    if (temp < 15.0) return -1;
    if (temp > 30.0) return 1;
    return 0;
}

DisplayMode nextDisplayMode(DisplayMode current) {
    if (current == MODE_MOTION) return MODE_TEMPERATURE;
    return static_cast<DisplayMode>(current + 1);
}

DisplayMode previousDisplayMode(DisplayMode current) {
    if (current == MODE_TEMPERATURE) return MODE_MOTION;
    return static_cast<DisplayMode>(current - 1);
}

SystemState evaluateSystemState(SystemState currentState, bool motion, bool timeout) {
    if (currentState == INACTIVE && motion) return ACTIVE;
    if (currentState == ACTIVE && timeout) return INACTIVE;
    return currentState;
}

void test_temp_below_lower(void)   { TEST_ASSERT_EQUAL(-1, evaluateTemperature(10.0)); }
void test_temp_exactly_lower(void) { TEST_ASSERT_EQUAL(0, evaluateTemperature(15.0)); }
void test_temp_normal(void)        { TEST_ASSERT_EQUAL(0, evaluateTemperature(25.4)); }
void test_temp_exactly_upper(void) { TEST_ASSERT_EQUAL(0, evaluateTemperature(30.0)); }
void test_temp_above_upper(void)   { TEST_ASSERT_EQUAL(1, evaluateTemperature(31.0)); }

void test_display_forward_transition(void) { TEST_ASSERT_EQUAL(MODE_HUMIDITY, nextDisplayMode(MODE_TEMPERATURE)); }
void test_display_forward_wraparound(void) { TEST_ASSERT_EQUAL(MODE_TEMPERATURE, nextDisplayMode(MODE_MOTION)); }
void test_display_reverse_transition(void) { TEST_ASSERT_EQUAL(MODE_LIGHT, previousDisplayMode(MODE_MOTION)); }
void test_display_reverse_wraparound(void) { TEST_ASSERT_EQUAL(MODE_MOTION, previousDisplayMode(MODE_TEMPERATURE)); }

void test_state_active_no_timeout(void)  { TEST_ASSERT_EQUAL(ACTIVE, evaluateSystemState(ACTIVE, false, false)); }
void test_state_active_timeout(void)     { TEST_ASSERT_EQUAL(INACTIVE, evaluateSystemState(ACTIVE, false, true)); }
void test_state_inactive_no_motion(void) { TEST_ASSERT_EQUAL(INACTIVE, evaluateSystemState(INACTIVE, false, false)); }
void test_state_inactive_motion(void)    { TEST_ASSERT_EQUAL(ACTIVE, evaluateSystemState(INACTIVE, true, false)); }

int main(int argc, char **argv) {
    UNITY_BEGIN();
    
    RUN_TEST(test_temp_below_lower);
    RUN_TEST(test_temp_exactly_lower);
    RUN_TEST(test_temp_normal);
    RUN_TEST(test_temp_exactly_upper);
    RUN_TEST(test_temp_above_upper);

    RUN_TEST(test_display_forward_transition);
    RUN_TEST(test_display_forward_wraparound);
    RUN_TEST(test_display_reverse_transition);
    RUN_TEST(test_display_reverse_wraparound);

    RUN_TEST(test_state_active_no_timeout);
    RUN_TEST(test_state_active_timeout);
    RUN_TEST(test_state_inactive_no_motion);
    RUN_TEST(test_state_inactive_motion);
    
    UNITY_END();
    return 0;
}