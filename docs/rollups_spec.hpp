/////////////////////
// Primitives

struct bytes {
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
    OPCODE_VERIFY_REPLAY_NOTICE_RESPOMSE,
    OPCODE_PREVIEW_REPLAY_INSPECT_REQUEST,
    OPCODE_ADD_CARTRIDGE_ADVANCE_REQUEST,
    OPCODE_REMOVE_CARTRIDGE_ADVANCE_REQUEST,
    OPCODE_DOWNLOAD_CARTRIDGE_INSPECT_REQUEST,
};

/////////////////////
// Advance requests

// Verify replay
struct verify_replay_advance_request {
    hash256_t opcode;
    hash256_t cartridge_hash;  // cartridge hash
    hash256_t resut_hash;      // result hash
    bytes args;                // command line parameters when running
    bytes card;                // memory card (extra parameters)
    bytes replay;              // replay log
} __attribute__((packed));

struct verify_replay_notice_response {
    hash256_t opcode;           // opcode
    hash256_t sender;           // sender address of the respective advance request
    hash256_t timestamp;        // timestamp of the respective advance request
    hash256_t cartridge_hash;   // cartridge of respective advance request
    hash256_t replay_hash;      // hash (args + card + replay) of respective advance request
    uint8_t status;             // replay status
    bool finished;              // whether the game finished
    int64_array result_scores;  // list of scores
    bytes result_card;          // result card (extra results)
} __attribute__((packed));

// Add cartridge
struct add_cartridge_advance_request {
    hash256_t opcode; // opcode
    bytes data;       // cartridge data
};
struct add_cartridge_report_response {
    uint8_t status; // opcode
    bytes hash;     // cartridge hash
} __attribute__((packed));

// Remove cartridge
struct remove_cartridge_advance_request {
    hash256_t opcode; // opcode
    bytes hash;       // cartridge hash
} __attribute__((packed));
struct remove_cartridge_report_response {
    uint8_t status;   // status
} __attribute__((packed));

/////////////////////
// Inspect requests

// Preview replay
using preview_replay_inspect_response = verify_replay_advance_request;
using preview_replay_report_response = verify_replay_notice_response;

// Download cartridge
struct download_cartridge_inspect_request {
    hash256_t opcode; // opcode
    bytes hash;       // cartridge hash
} __attribute__((packed));
struct download_cartridge_report_response {
    uint8_t status;  // status
    bytes data;      // cartridge data
} __attribute__((packed));
