using DS_LV2;
using Google.Protobuf.WellKnownTypes;
using Grpc.Core;

namespace DS_LV2_S.Services
{
    public class MathServiceImpl : MathService.MathServiceBase
    {
        private static double acc = 1;

        public override Task<Empty> UpdateAcc(NumberRequest request, ServerCallContext context)
        {
            int n = request.Value;

            double sum = 0;

            for (int i = 1; i <= n; i++)
                sum += i;

            double avg = sum / n;

            acc += avg;

            Console.WriteLine($"acc = {acc}");
            return Task.FromResult(new Empty());
        }

        public override async Task ProcessStream(IAsyncStreamReader<NumberRequest> requestStream, IServerStreamWriter<NumberResponse> responseStream, ServerCallContext context)
        {
            int counter = 0;

            await foreach (var item in requestStream.ReadAllAsync())
            {
                counter++;

                int result;

                if (counter % 3 == 0)
                {
                    result = (int)(item.Value * acc);
                }
                else
                {
                    result = (int)(item.Value - acc);
                }

                await responseStream.WriteAsync(
                new NumberResponse
                {
                    Value = result
                });
            }
        }
    }
}