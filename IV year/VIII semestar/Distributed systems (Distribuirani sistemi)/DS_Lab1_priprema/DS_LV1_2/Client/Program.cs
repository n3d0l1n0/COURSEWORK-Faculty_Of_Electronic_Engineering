using DS_LV2;
using Grpc.Core;
using Grpc.Net.Client;

var channel = GrpcChannel.ForAddress("http://localhost:5058");
var client = new MathService.MathServiceClient(channel);

await client.UpdateAccAsync(
new NumberRequest
{
    Value = 5
});

Console.WriteLine("acc apdejtovan");

using var call = client.ProcessStream();
int[] numbers = { 10,20,30,40,50,60};

foreach (var n in numbers)
{
    await call.RequestStream.WriteAsync(
    new NumberRequest
    {
            Value = n
    });
}

await call.RequestStream.CompleteAsync();

await foreach(var response in call.ResponseStream.ReadAllAsync())
{
    Console.WriteLine(response.Value);
}