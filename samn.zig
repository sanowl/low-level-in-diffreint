const std = @import("std");
const Allocator = std.mem.Allocator;
const Thread = std.Thread;
const net = std.net;
const os = std.os;

fn threadFunction(data: anytype) void {
    var thread_id: usize = os.getThreadId();
    std.debug.print("Thread started with data: {}, Thread ID: {}\n", .{data, thread_id});

    // Perform thread-specific operations here...
}

fn runServer() !void {
    const allocator = std.heap.page_allocator;
    var server = try net.TcpServer.init(allocator);
    defer server.deinit();

    try server.listen(net.Address.parseIp4("127.0.0.1", 8080), 10);
    std.debug.print("Server listening on port 8080\n", .{});

    while (true) {
        const connection = try server.accept();
        std.debug.print("New connection: {}\n", .{connection});
        connection.close();
    }
}

pub fn main() !void {
    const allocator = std.heap.page_allocator;

    // Allocating memory
    const size: usize = 1024;
    const memory = try allocator.alloc(u8, size);
    defer allocator.free(memory);

    // Fill the memory with some data
    for (memory) |*byte, index| {
        byte.* = @intToPtr(*u8, index % 255);
    }

    // Launching a thread
    var thread = try Thread.create("WorkerThread", threadFunction, 42);
    defer thread.wait() catch unreachable;

    // Making a system call (POSIX example)
    const pid = os.getpid();
    std.debug.print("Process ID: {}\n", .{pid});

    // Run TCP Server
    try runServer();
}
