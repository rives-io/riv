/////////////////////
// Primitives

struct string {
    uint32_t size;
    uint8_t data[];
} __attribute__((packed));

struct int64_array {
    uint16_t size;
    int64_t data[];
} __attribute__((packed));

/////////////////////
// Enums

enum response_status : uint8_t {
    STATUS_SUCCESS = 0,
    STATUS_RESULT_HASH_MISMATCH,
    STATUS_CARTRIDGE_NOT_FOUND,
    STATUS_CPU_TIME_EXCEEDED,
    STATUS_KILLED,
    STATUS_RUNTIME_ERROR
};

enum request_opcode : uint8_t {
    OPCODE_VERIFY_REPLAY_ADVANCE_REQUEST,
    OPCODE_PREVIEW_REPLAY_INSPECT_REQUEST,
    // OPCODE_ADD_CARTRIDGE_ADVANCE_REQUEST,
    // OPCODE_REMOVE_CARTRIDGE_ADVANCE_REQUEST,
    // OPCODE_DOWNLOAD_CARTRIDGE_INSPECT_REQUEST,
};

/////////////////////
// Advance requests

// Verify replay
struct verify_replay_advance_request {
    uint8_t opcode;
    hash256_t cartridge_hash;   // cartridge hash
    hash256_t resut_hash;       // result hash
    string args;                // command line parameters when running
    string card;                // memory card (extra parameters)
    string replay;              // compressed replay
} __attribute__((packed));
struct verify_replay_notice_response {
    uint8_t status;             // replay status
    bool finished;              // whether the game finished
    int64_array result_scores;  // list of scores
    string result_card;         // result card (extra results)
} __attribute__((packed));

/*
// Add cartridge
struct add_cartridge_advance_request {
    uint8_t opcode; // opcode
    string data;    // cartridge data
};
struct add_cartridge_report_response {
    uint8_t status; // opcode
    string hash;    // cartridge hash
};

// Remove cartridge
struct remove_cartridge_advance_request {
    uint8_t opcode; // opcode
    string hash;    // cartridge hash
};
struct remove_cartridge_report_response {
    uint8_t status; // status
};
*/

/////////////////////
// Inspect requests

// Preview replay
using preview_replay_inspect_response = verify_replay_advance_request;
using preview_replay_report_response = verify_replay_notice_response;

/*
// Download cartridge
struct download_cartridge_advance_request {
    uint8_t opcode; // opcode
    string hash;    // cartridge hash
};
struct download_cartridge_report_response {
    uint8_t status; // status
    string data;    // cartridge data
};
*/
