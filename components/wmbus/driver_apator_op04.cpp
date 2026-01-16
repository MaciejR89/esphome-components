#include "meters_common_implementation.h"

namespace
{
    struct Driver : public virtual MeterCommonImplementation
    {
        Driver(MeterInfo &mi, DriverInfo &di);

    private:
        void processContent(Telegram *t);
    };

    static bool ok = registerDriver([](DriverInfo& di)
    {
        di.setName("apator_op04");
        di.setDefaultFields("name,id,total,timestamp");
        di.setMeterType(MeterType::WaterMeter);
        di.addLinkMode(LinkMode::T1);

        di.addDetection(MANUFACTURER_APA, 0x1A, 0x07);

        di.usesProcessContent();
        di.setConstructor([](MeterInfo& mi, DriverInfo& di)
        {
            return std::shared_ptr<Meter>(new Driver(mi, di));
        });
    });

    Driver::Driver(MeterInfo &mi, DriverInfo &di)
        : MeterCommonImplementation(mi, di)
    {
        addNumericField(
            "total",
            Quantity::Volume,
            DEFAULT_PRINT_PROPERTIES,
            "Total water consumption");
    }

    void Driver::processContent(Telegram *t)
    {
        t->dll_type = 0x07;

        std::vector<uchar> content;
        t->extractPayload(&content);

        if (content.size() < 6)
            return;

        // DIF=04, VIF=13 → Volume (liters)
        if (content[0] == 0x04 && content[1] == 0x13)
        {
            uint32_t raw =
                content[2] |
                (content[3] << 8) |
                (content[4] << 16) |
                (content[5] << 24);

            // raw = litry → m³
            double total_m3 = raw / 1000.0;

            setNumericValue("total", Unit::M3, total_m3);

            std::string info;
            strprintf(&info, "total raw %u L → %f m3", raw, total_m3);
            t->addSpecialExplanation(2, 4, KindOfData::CONTENT,
                                     Understanding::FULL,
                                     info.c_str(),
                                     raw);
        }
    }
}
