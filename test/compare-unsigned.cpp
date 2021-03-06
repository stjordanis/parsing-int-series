#include <vector>
#include <iterator>
#include <cstdio>
#include <cstdlib>

#include "input_generator.h"
#include "scalar/scalar-parse-unsigned.h"
#include "sse/sse-matcher.h"
#include "sse/sse-parser-unsigned.h"

#include "application.h"

class CompareApp: public Application {

    using Vector = std::vector<uint32_t>;

public:
    CompareApp(int argc, char** argv) : Application(argc, argv) {}

private:
    virtual bool custom_run() override;

private:
    void dump(const Vector& vec) const;
    bool compare(const Vector& expected, const Vector& result) const;

};

bool CompareApp::custom_run() {

    const auto tmp = generate_unsigned();

    Vector reference;
    Vector result;
    const char* separators = ";, ";
    scalar::parse_unsigned(tmp.data(), tmp.size(), separators, std::back_inserter(reference));

    sse::NaiveMatcher<8> matcher(separators);
    sse::parser(tmp.data(), tmp.size(), separators, std::move(matcher), std::back_inserter(result));

    if (!compare(reference, result)) {
        puts(tmp.c_str());
        puts("");
        dump(reference);
        puts("");
        dump(result);

        return false;
    } else {
        puts("All OK");
        return true;
    }
}

void CompareApp::dump(const Vector& vec) const {
    printf("size = %lu: [", vec.size());

    const size_t n = vec.size();
    if (n) {
        printf("%u", vec[0]);
    }

    for (size_t i=1; i < n; i++) {
        printf(", %u", vec[i]);
    }

    printf("]\n");
}

bool CompareApp::compare(const Vector& expected, const Vector& result) const {

    if (expected.size() != result.size()) {
        puts("different sizes");
        return false;
    }

    const size_t n = expected.size();
    for (size_t i=0; i < n; i++) {
        const auto e = expected[i];
        const auto r = result[i];

        if (e != r) {
            printf("error at #%lu: expected = %u, result = %d\n", i, e, r);
            return false;
        }
    }

    return true;
}

int main(int argc, char* argv[]) {

    try {
        CompareApp app(argc, argv);

        return app.run() ? EXIT_SUCCESS : EXIT_FAILURE;

    } catch (std::exception& e) {
        printf("%s\n", e.what());
        return EXIT_FAILURE;
    } catch (Application::Exit&) {
        return EXIT_SUCCESS;
    }
}

