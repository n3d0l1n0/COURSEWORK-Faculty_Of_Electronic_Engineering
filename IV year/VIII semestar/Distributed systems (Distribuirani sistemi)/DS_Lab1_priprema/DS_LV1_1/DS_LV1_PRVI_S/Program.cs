using DS_LV1_PRVI.Services;

var builder = WebApplication.CreateBuilder(args);

builder.Services.AddGrpc();

var app = builder.Build();

app.MapGrpcService<UserServiceImpl>();

app.MapGet("/", () =>
{
    return "gRPC server running";
});

app.Run();
